#include "table.h"
#include "columns.h"
#include <stdlib.h>
#include <string.h>

/*
 * TableNode
 */

TableNode *new_table_node(const char *key, ColumnsHashTable *val) {
  TableNode *node = malloc(sizeof(TableNode));

  strncpy(node->key, key, TBL_NAME_LEN_MAX);
  node->key[TBL_NAME_LEN_MAX] = '\0';

  node->val = val;

  node->next = NULL;
  node->prev = NULL;

  return node;
}

/*
 * TableLinkedList
 */

TableLinkedList *new_tables_linked_list() {
  TableLinkedList *tll = malloc(sizeof(TableLinkedList));
  tll->head = NULL;
  tll->tail = NULL;

  return tll;
}

void free_tbls_ll(TableLinkedList *ll) {
  TableNode *cur = ll->head;

  while (cur != NULL) {
    TableNode *tmp = cur;
    cur = cur->next;
    free(tmp);
  }

  free(ll);
}

void tbls_ll_append(TableLinkedList *tll, TableNode *node) {
  if (tll->tail == NULL || node == NULL) {
    return;
  }

  tll->tail->next = node;

  node->prev = tll->tail;

  tll->tail = node;
}

void tbls_ll_pop(TableLinkedList *tll) {
  if (tll->tail == NULL) {
    return;
  }

  if (tll->head != tll->tail) {
    tll->tail = tll->tail->prev;

    free_col_ht(tll->tail->next->val);

    tll->tail->next = NULL;
  } else {
    free_col_ht(tll->head->val);

    tll->head = NULL;

    tll->tail = NULL;
  }
}

ColumnsHashTable *tbls_ll_search(TableLinkedList *ll, char *key) {
  TableNode *cur = ll->head;

  while (cur != NULL) {
    if (strcmp(cur->key, key)) {
      return cur->val;
    }

    cur = cur->next;
  }

  return NULL;
}

/*
 * TablesHashTable
 */

TablesHashTable *new_tables_hash_table(int (*hash_func)(const char *)) {
  TablesHashTable *ht = malloc(sizeof(TablesHashTable));

  for (int i = 0; i < TABLES_HT_SIZE; i++) {
    ht->arr[i] = NULL;
  }

  ht->hash_func = hash_func;

  return ht;
}

void free_tbls_ht(TablesHashTable *ht) {
  for (int i = 0; i < TABLES_HT_SIZE; i++) {
    if (ht->arr[i] != NULL) {
      free_tbls_ll(ht->arr[i]);
    }
  }

  free(ht);
}

void tbls_ht_put(TablesHashTable *ht, char *key, ColumnsHashTable *val) {
  int index = ht->hash_func(key);

  if (ht->arr[index] == NULL) {
    ht->arr[index] = new_tables_linked_list();
  }

  tbls_ll_append(ht->arr[index], new_table_node(key, val));
}

void *tbls_ht_get(TablesHashTable *ht, char *key) {
  int index = ht->hash_func(key);

  if (ht->arr[index] == NULL) {
    return NULL;
  }

  return tbls_ll_search(ht->arr[index], key);
}
