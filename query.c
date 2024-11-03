#include "query.h"
#include "str_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum QueryOperation parse_oper_type(char **ptr) {
  char *buf_ptr = *ptr;

  while (**ptr != '\0' && **ptr != ' ') {
    (*ptr)++;
  }

  if (substrcmp(buf_ptr, *ptr, "CREATE")) {
    return Q_OPER_CREATE;
  } else if (substrcmp(buf_ptr, *ptr, "INSERT")) {
    return Q_OPER_INSERT;
  } else if (substrcmp(buf_ptr, *ptr, "SELECT")) {
    return Q_OPER_SELECT;
  } else {
    return -1;
    printf("failed define operation");
  }
}

char *parse_create_target(char **ptr) {
  // skip spaces
  while (**ptr == ' ') {
    (*ptr)++;
  }

  char *start_ptr = *ptr;

  int i = 0;
  while (**ptr != '\0' && **ptr != ' ') {
    (*ptr)++;
    i++;
  }

  char *res = malloc(sizeof(char) * (i + 1));

  strncpy(res, start_ptr, i);

  res[i] = '\0';

  return res;
}

char *parse_preworded_target(char **ptr, const char *preword) {
  // skip spaces
  while (**ptr == ' ') {
    (*ptr)++;
  }

  char *start_ptr = *ptr;

  while (**ptr != '\0' && **ptr != ' ') {
    (*ptr)++;
  }

  if (!substrcmp(start_ptr, *ptr, preword)) {
    printf("not found preword %s\n", preword);
    return NULL;
  }

  // skip spaces
  while (**ptr == ' ') {
    (*ptr)++;
  }

  start_ptr = *ptr;

  int i = 0;
  while (**ptr != '\0' && **ptr != ' ') {
    (*ptr)++;
    i++;
  }

  char *res = malloc(sizeof(char) * (i + 1));

  strncpy(res, start_ptr, i);

  res[i] = '\0';

  return res;
}

char *parse_target(char **ptr, enum QueryOperation q_oper) {
  char *res;
  res = NULL;

  // TODO: handle select and insert
  switch (q_oper) {
  case Q_OPER_CREATE:
    return parse_create_target(ptr);
  case Q_OPER_INSERT:
    return parse_preworded_target(ptr, "INTO");
  case Q_OPER_SELECT:
    return parse_preworded_target(ptr, "FROM");
  default:
    return res;
  }
}

// TODO: implment and rework everything from indexes into pointers arithmetics
ColumnTypeName *parse_table_structure_definition(char *q, int start_idx) {
  ColumnTypeName *columns_arr = malloc(sizeof(ColumnTypeName));

  if (start_idx < 0 || start_idx >= strlen(q)) {
    free(columns_arr);
    return NULL;
  }

  int i = start_idx;
  char *ptr = q;

  if (*ptr != '(') {
    free(columns_arr);
    return NULL;
  }

  while (*ptr != '\0' && *ptr != ' ') {
    ptr++;
    i++;
  }

  return columns_arr;
}

// ColumnTypeName parse_next_definition() {}
