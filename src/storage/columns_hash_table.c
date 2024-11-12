#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLUMNS_HT_SIZE 10

#define STR_SIZE 11

typedef struct ColumnsHashTable {
  void *arr[COLUMNS_HT_SIZE];
  int (*hash_func)(const char *);
  size_t val_size;
} ColumnsHashTable;

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

void free_columns_ht(ColumnsHashTable *ht) {
  for (int i = 0; i < COLUMNS_HT_SIZE; i++) {
    free(ht->arr[i]);
  }

  free(ht);
}

void put(ColumnsHashTable *ht, char *key, void *val) {
  int index = ht->hash_func(key);

  int i = 0;
  while (ht->arr[index] != NULL) {
    if (i >= COLUMNS_HT_SIZE) {
      printf("infinite probing in put");
      exit(-1);
    }

    index = (index + 1) % COLUMNS_HT_SIZE;
    i++;
  }

  ht->arr[index] = val;
}

void *get(ColumnsHashTable *ht, char *key) {
  int index = ht->hash_func(key);

  int i = 0;
  while (strcmp(key, ht->arr[index]) != 0) {
    if (i >= COLUMNS_HT_SIZE) {
      printf("infinite probing in get");
      exit(-1);
    }
    index = (index + 1) % COLUMNS_HT_SIZE;
    i++;
  }

  return ht->arr[index];
}
