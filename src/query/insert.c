#include <stdio.h>
#include <stdlib.h>

#include "../util/str_utils.h"

char **parse_columns_to_insert(char **q_ptr, int *columns_count) {
  while (**q_ptr != '(' && **q_ptr != '\0') {
    (*q_ptr)++;
  }

  if (**q_ptr == '\0') {
    printf("couldn't find opening parenthesis\n");
    return NULL;
  } else {
    (*q_ptr)++;
  }

  *columns_count = 0;

  char **substrs = get_substrs(q_ptr, columns_count, ')');
  if (substrs == NULL) {
    printf("missing columns to insert definitions\n");
    return NULL;
  }

  for (int i = 0; i < *columns_count; i++) {
    char *truncated_substr = truncate_bounding_space(substrs[i]);
    free(substrs[i]);
    substrs[i] = truncated_substr;
  }

  return substrs;
}

// TODO: come up with way and structures for handling insert and then rework
// input params
void parse_values_to_insert(char **q_ptr, char **column_names,
                            int columns_count) {
  while (**q_ptr == ' ') {
    (*q_ptr)++;
  }

  if (**q_ptr == '\0') {
    // TODO: think about more appropriate log
    printf("couldn't find values keyword");
    // return NULL;
  }
}
