#pragma once

#include "columns.h"
#include <stddef.h>

#define TABLES_HT_SIZE 10

typedef struct TableNode {
  char key[TBL_NAME_LEN_MAX + 1];
  ColumnsHashTable *val;
  struct TableNode *next;
  struct TableNode *prev;
} TableNode;

TableNode *new_table_node(const char *key, ColumnsHashTable *val);

typedef struct TableLinkedList {
  TableNode *head;
  TableNode *tail;
} TableLinkedList;

TableLinkedList *new_tables_linked_list();

void free_tbls_ll(TableLinkedList *ll);

void tbls_ll_append(TableLinkedList *tll, TableNode *node);

void tbls_ll_pop(TableLinkedList *tll);

ColumnsHashTable *tbls_ll_search(TableLinkedList *ll, char *key);

typedef struct TablesHashTable {
  TableLinkedList *arr[TABLES_HT_SIZE];
  int (*hash_func)(const char *);
} TablesHashTable;

TablesHashTable *new_tables_hash_table(int (*hash_func)(const char *));

void free_tbls_ht(TablesHashTable *ht);

void tbls_ht_put(TablesHashTable *ht, char *key, ColumnsHashTable *val);

void *tbls_ht_get(TablesHashTable *ht, char *key);
