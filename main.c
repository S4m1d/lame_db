#include "processor.h"
#include "query.h"
#include "str_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void truncate_nlc(char *);

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

void truncate_nlc(char *str) {
  char *new_line_ch_pos = strchr(str, '\n');

  if (new_line_ch_pos != NULL) {
    *new_line_ch_pos = '\0';
  }
}
