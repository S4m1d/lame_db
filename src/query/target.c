#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/str_utils.h"

char *parse_preworded_target(char **q_ptr, const char *preword) {
  int skip_res = skip_word(q_ptr, preword);
  if (skip_res != 0) {
    return NULL;
  }

  // skip spaces
  skip_spaces(q_ptr);

  char *start_ptr = *q_ptr;

  int i = 0;
  while (**q_ptr != '\0' && **q_ptr != ' ') {
    (*q_ptr)++;
    i++;
  }

  char *res = malloc(sizeof(char) * (i + 1));
  if (res == NULL) {
    printf("failed allocate memory");
    exit(EXIT_FAILURE);
  }

  strncpy(res, start_ptr, i);

  res[i] = '\0';

  return res;
}
