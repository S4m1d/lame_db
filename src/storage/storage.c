#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../util/str_utils.h"
#include "hash_table/columns.h"
#include "hash_table/table.h"
#include "storage.h"
#include "table.h"

int tbls_hash_func(const char *key) {
  if (key == NULL) {
    return 0;
  }

  char *cur = key;

  unsigned int sum = 0;

  int i = 1;
  while (*cur != '\0') {
    sum += ((unsigned int)*cur) * i;
    cur++;
    i++;
  }

  return sum % TBLS_HASH_FUNC_MODULUS;
}

int col_def_hash_func(const char *key) {
  if (key == NULL) {
    return 0;
  }

  char *cur = key;

  unsigned int sum = 0;

  int i = 1;
  while (*cur != '\0') {
    sum += ((unsigned int)*cur) * i;
    cur++;
    i++;
  }

  return sum % COL_HASH_FUNC_MODULUS;
}

TablesHashTable *scan_tables() {
  printf("SCAN START\n");

  DIR *storage_dir = opendir(STORAGE_PATH);
  if (storage_dir == NULL) {
    printf("failed to open dir %s\n", STORAGE_PATH);
    return NULL;
  }

  chdir(STORAGE_PATH);

  struct stat statbuf;

  TablesHashTable *tbls_ht = new_tables_hash_table(tbls_hash_func);

  struct dirent *dir_ent = readdir(storage_dir);

  int iter_cnt = 0;
  while ((dir_ent) != NULL) {
    lstat(dir_ent->d_name, &statbuf);
    printf("iteration %d\n", iter_cnt);

    if (S_ISREG(statbuf.st_mode)) {
      printf("iteration %d, regular file found\n", iter_cnt);
      char *ext_beg_ptr = find_first(dir_ent->d_name, '.');
      if (substrcmp(ext_beg_ptr, NULL, ".ldbt")) {
        char tbl_name[TBL_NAME_LEN_MAX + 1];
        int tbl_name_len = ext_beg_ptr - dir_ent->d_name;

        if (tbl_name_len > TBL_NAME_LEN_MAX) {
          printf("file name too long\n");
          exit(-1);
        } else if (tbl_name_len < TBL_NAME_LEN_MAX) {
          strncpy(tbl_name, dir_ent->d_name, tbl_name_len);
        } else {
          strncpy(tbl_name, dir_ent->d_name, tbl_name_len);
          tbl_name[TBL_NAME_LEN_MAX] = '\0';
        }

        int col_count = 0;

        PosColumnDefinition **column_defs =
            read_table_definition(tbl_name, &col_count);
        if (column_defs == NULL) {
          closedir(storage_dir);
          return NULL;
        }

        ColumnsHashTable *col_ht = new_columns_hash_table(
            sizeof(PosColumnDefinition), col_def_hash_func);

        // TODO: remove output after debug;
        printf("table %s, colums count %d\n", tbl_name, col_count);

        for (int i = 0; i < col_count; i++) {
          printf("column %d: name %s, type %s\n", column_defs[i]->pos,
                 column_defs[i]->info.name, dt_names[column_defs[i]->info.t]);

          col_ht_put(col_ht, column_defs[i]->info.name, column_defs[i]);
        }

        tbls_ht_put(tbls_ht, tbl_name, col_ht);
      }
    }

    dir_ent = readdir(storage_dir);
    // TODO: remove after debug
    iter_cnt++;
  }

  closedir(storage_dir);

  printf("SCAN END\n");
  return tbls_ht;
}
