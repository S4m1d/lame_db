#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "columns.h"

void free_col_ht_entry(ColumnsHtEntry *entry) {
  free(entry->val);
  free(entry);
}

ColumnsHashTable *new_columns_hash_table(size_t val_size,
                                         int (*hash_func)(const char *)) {
  ColumnsHashTable *ht = malloc(sizeof(ColumnsHashTable));

  ht->val_size = val_size;
  ht->hash_func = hash_func;

  for (int i = 0; i < COLUMNS_HT_SIZE; i++) {
    ht->arr[i] = NULL;
  }

  return ht;
}

void free_col_ht(ColumnsHashTable *ht) {
  for (int i = 0; i < COLUMNS_HT_SIZE; i++) {
    free_col_ht_entry(ht->arr[i]);
  }

  free(ht);
}

void col_ht_put(ColumnsHashTable *ht, char *key, void *val) {
  int index = ht->hash_func(key);

  for (int i = 0; i < COLUMNS_HT_SIZE; i++) {
    if (ht->arr[index] == NULL) {
      ColumnsHtEntry *entry = malloc(sizeof(ColumnsHtEntry));

      strncpy(entry->key, key, COL_NAME_LEN_MAX - 1);
      entry->key[COL_NAME_LEN_MAX] = '\0';

      entry->val = val;

      ht->arr[index] = entry;

      return;
    } else if (strcmp(ht->arr[index]->key, key)) {
      free_col_ht_entry(ht->arr[index]);

      ColumnsHtEntry *entry = malloc(sizeof(ColumnsHtEntry));

      strncpy(entry->key, key, COL_NAME_LEN_MAX - 1);
      entry->key[COL_NAME_LEN_MAX] = '\0';

      entry->val = val;

      ht->arr[index] = entry;

      return;
    } else {
      index = (index + 1) % COLUMNS_HT_SIZE;
    }
  }

  printf("infinite probing in columns ht put");
  exit(-1);
}

void *col_ht_get(ColumnsHashTable *ht, char *key) {
  int index = ht->hash_func(key);

  for (int i = 0; i < COLUMNS_HT_SIZE; i++) {
    if (ht->arr[index] != NULL && strcmp(ht->arr[index]->key, key)) {
      return ht->arr[index]->val;
    } else {
      index = (index + 1) % COLUMNS_HT_SIZE;
    }
  }

  printf("infinite probing in columns ht get");
  exit(-1);
}
