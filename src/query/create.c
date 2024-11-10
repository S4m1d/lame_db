#include "create.h"
#include "../util/str_utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *BAD_COL_DEF_ERR_TMPLT =
    "bad column definition: expected <${data type} ${column name}>, got <%s>";
int parse_column_definition(char *ptr, ColumnDefinition *dest) {
  // skip empty space
  char *cur_ptr = ptr;
  while (*cur_ptr == ' ') {
    cur_ptr++;
  }

  if (*cur_ptr == '\0') {
    printf(BAD_COL_DEF_ERR_TMPLT, ptr);

    return -1;
  }

  char *start_ptr = cur_ptr;

  while (*cur_ptr != ' ' && *cur_ptr != '\0') {
    cur_ptr++;
  }

  if (*cur_ptr == '\0') {
    printf(BAD_COL_DEF_ERR_TMPLT, ptr);

    return -1;
  }

  if (substrcmp(start_ptr, cur_ptr, "INT")) {
    dest->t = DT_INT;
  } else if (substrcmp(start_ptr, cur_ptr, "TEXT")) {
    dest->t = DT_TEXT;
  }

  while (*cur_ptr == ' ') {
    cur_ptr++;
  }

  if (*cur_ptr == '\0') {
    printf(BAD_COL_DEF_ERR_TMPLT, ptr);

    return -1;
  }

  start_ptr = cur_ptr;

  while (*cur_ptr != ' ' && *cur_ptr != '\0') {
    cur_ptr++;
  }

  int name_len = cur_ptr - start_ptr;

  if (name_len > COL_NAME_LENGTH_MAX - 1) {
    printf("col name too long (>50)");

    return -1;
  }

  strncpy(dest->name, start_ptr, name_len);
  dest->name[name_len] = '\0';

  return 0;
}

const char *data_type_str[] = {
    "INT",
    "TEXT",
};

ColumnDefinition *parse_table_definition(char **q_ptr, int *col_defs_count) {
  // find opening parenthesis
  while (**q_ptr != '(' && **q_ptr != '\0') {
    (*q_ptr)++;
  }

  if (**q_ptr == '\0') {
    printf("couldn't find opening parenthesis");
    return NULL;
  } else {
    (*q_ptr)++;
  }

  int substrs_count = 0;
  char **substrs = get_substrs(q_ptr, &substrs_count, ',', ')');
  if (substrs == NULL) {
    printf("missing table structure definition\n");
    return NULL;
  }

  *col_defs_count = 0;
  ColumnDefinition *column_defs =
      malloc(sizeof(ColumnDefinition) * substrs_count);

  for (int i = 0; i < substrs_count; i++) {
    // TODO: remove after debug
    printf("substr %d: %s\n", i, substrs[i]);

    int res =
        parse_column_definition(substrs[i], &column_defs[*col_defs_count]);

    if (res != 0) {
      free(column_defs);

      return NULL;
    }

    (*col_defs_count)++;
  }

  // TODO: remove after debug
  for (int i = 0; i < *col_defs_count; i++) {
    printf("column %d: name=%s type=%s\n", i, column_defs[i].name,
           data_type_str[column_defs[i].t]);
  }

  free_substrs_arr(substrs, substrs_count);

  return column_defs;
}
