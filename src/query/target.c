#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "../util/str_utils.h"

char *parse_create_target(char **q_ptr) {
  // skip spaces
  while (**q_ptr == ' ') {
    (*q_ptr)++;
  }

  char *start_ptr = *q_ptr;

  int i = 0;
  while (**q_ptr != '\0' && **q_ptr != ' ') {
    (*q_ptr)++;
    i++;
  }

  char *res = malloc(sizeof(char) * (i + 1));

  strncpy(res, start_ptr, i);

  res[i] = '\0';

  return res;
}

char *parse_preworded_target(char **q_ptr, const char *preword) {
  // skip spaces
  while (**q_ptr == ' ') {
    (*q_ptr)++;
  }

  char *start_ptr = *q_ptr;

  while (**q_ptr != '\0' && **q_ptr != ' ') {
    (*q_ptr)++;
  }

  if (!substrcmp(start_ptr, *q_ptr, preword)) {
    printf("not found preword %s\n", preword);
    return NULL;
  }

  // skip spaces
  while (**q_ptr == ' ') {
    (*q_ptr)++;
  }

  start_ptr = *q_ptr;

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

char *parse_target(char **q_ptr, enum QueryOperation q_oper) {
  char *res;
  res = NULL;

  switch (q_oper) {
  case Q_OPER_CREATE:
    return parse_create_target(q_ptr);
  case Q_OPER_INSERT:
    return parse_preworded_target(q_ptr, "INTO");
  case Q_OPER_SELECT:
    return parse_preworded_target(q_ptr, "FROM");
  default:
    return res;
  }
}
