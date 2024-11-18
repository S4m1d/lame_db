#pragma once

#include <stddef.h>
#include "../../common/limits.h"

#define COLUMNS_HT_SIZE 10

#define STR_SIZE 11

typedef struct ColumnsHtEntry {
  char key[COL_NAME_LEN_MAX+1];
  void *val;
} ColumnsHtEntry;

typedef struct ColumnsHashTable {
  ColumnsHtEntry *arr[COLUMNS_HT_SIZE];
  int (*hash_func)(const char *);
  size_t val_size;
} ColumnsHashTable;

ColumnsHashTable *new_columns_hash_table(size_t val_size,
                                         int (*hash_func)(const char *));

void free_col_ht(ColumnsHashTable *ht);

void col_ht_put(ColumnsHashTable *ht, char *key, void *val);

void *col_ht_get(ColumnsHashTable *ht, char *key);
