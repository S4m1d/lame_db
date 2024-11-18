#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processor.h"
#include "storage/hash_table/table.h"
#include "storage/storage.h"
#include "util/str_utils.h"

#define MAX_QUERY_SIZE 100

int main() {
  bool is_terminated = false;

  TablesHashTable *tbls_ht = scan_tables();
  if (tbls_ht == NULL) {
    printf("failed init context\n");
    exit(-1);
  }

  while (!is_terminated) {
    char query[MAX_QUERY_SIZE];

    fgets(query, sizeof(query), stdin);

    truncate_nlc(query);

    if (strcmp(query, "\\exit") == 0) {
      break;
    }

    process_query(query);
  }

  free_tbls_ht(tbls_ht);

  return 0;
}
