#include "processor.h"
#include "util/str_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_QUERY_SIZE 100

int main() {
  bool is_terminated = false;
  while (!is_terminated) {
    char query[MAX_QUERY_SIZE];

    fgets(query, sizeof(query), stdin);

    truncate_nlc(query);

    if (strcmp(query, "\\exit") == 0) {
      break;
    }

    process_query(query);
  }

  return 0;
}
