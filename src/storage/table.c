#include "../common/types.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_table(const char *table_name, ColumnDefinition *column_defs,
                 int columns_count) {

  // explanation of 7: 1(slash) + 5(.ldbt) + \0
  char name[strlen(STORAGE_PATH) + strlen(table_name) + 7];

  snprintf(name, sizeof(name), "%s/%s.ldbt", STORAGE_PATH, table_name);

  FILE *f = fopen(name, "wb");
  if (f == NULL) {
    printf("failed open/create file %s\n", name);
    return -1;
  }

  // writing number of columns at the beginning of the file
  int write_res = fwrite(&columns_count, sizeof(int), 1, f);
  if (write_res < 0) {
    printf("failed write to file %s, code %d\n", name, write_res);
    exit(write_res);
  }

  for (int i = 0; i < columns_count; i++) {
    int write_res = fwrite(&column_defs[i], sizeof(ColumnDefinition), 1, f);
    if (write_res < 0) {
      printf("failed write to file %s, code %d\n", name, write_res);
      exit(write_res);
    }
  }

  int close_res = fclose(f);
  if (close_res != 0) {
    printf("failed to close file %s, code %d\n", name, close_res);
    exit(-close_res);
  }

  return 0;
}

PosColumnDefinition **read_table_definition(const char *table_name,
                                            int *columns_count) {

  char name[strlen(table_name) + 6];

  snprintf(name, sizeof(name), "%s.ldbt", table_name);

  FILE *f = fopen(name, "rb");
  if (f == NULL) {
    printf("failed open/create file %s\n", name);
    return NULL;
  }

  *columns_count = 0;
  int read_res = fread(columns_count, sizeof(int), 1, f);
  if (read_res < 0) {
    printf("failed read columns count in file %s, code %d\n", name, read_res);
    exit(read_res);
  }

  PosColumnDefinition **column_defs = malloc(*columns_count * sizeof(void *));

  for (int i = 0; i < *columns_count; i++) {
    if (feof(f)) {
      printf("DATA CORRUPTION: read_table_definition, %s, eof reached earlier, "
             "than "
             "expected",
             name);
      exit(-1);
    }

    PosColumnDefinition *col_def = malloc(sizeof(PosColumnDefinition));

    read_res = fread(&col_def->info, sizeof(ColumnDefinition), 1, f);
    if (read_res < 0) {
      printf("failed read column definitions in file %s, code %d\n", name,
             read_res);
      exit(read_res);
    }

    col_def->pos = i;

    column_defs[i] = col_def;
  }

  int close_res = fclose(f);
  if (close_res != 0) {
    printf("failed to close file %s, code %d\n", name, close_res);
    exit(-close_res);
  }

  return column_defs;
}
