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
  if (res == NULL) {
    printf("failed allocate memory");
    exit(EXIT_FAILURE);
  }

  strncpy(res, start_ptr, i);

  res[i] = '\0';

  return res;
}

char *parse_target(char **ptr, enum QueryOperation q_oper) {
  char *res;
  res = NULL;

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

void write_substr(char ***substr_arr, int *count, char *substr_beg_ptr,
                  char *substr_end_ptr) {

  int substr_len = substr_end_ptr - substr_beg_ptr;

  char *substr = (char *)malloc((substr_len + 1) * sizeof(char));
  if (substr == NULL) {
    printf("failed allocate memory for substring\n");
    exit(EXIT_FAILURE);
  }

  strncpy(substr, substr_beg_ptr, substr_len);
  substr[substr_len] = '\0';

  if (*count == 0) {
    *substr_arr = (char **)malloc(1 * sizeof(void *));
    if (*substr_arr == NULL) {
      printf("failed allocate memory for substring\n");
      exit(EXIT_FAILURE);
    }
  } else {
    *substr_arr = realloc(*substr_arr, (*count + 1) * sizeof(void *));
    if (*substr_arr == NULL) {
      printf("failed reallocate memory for substring\n");
      exit(EXIT_FAILURE);
    }
  }

  (*substr_arr)[*count] = substr;

  (*count)++;
}

char **get_substrs(char **ptr, int *count) {
  *count = 0;
  char **substr_arr;

  char *substr_beg_ptr = *ptr;
  while (**ptr != '\0') {
    if (**ptr == ',') {
      write_substr(&substr_arr, count, substr_beg_ptr, *ptr);
      substr_beg_ptr = ++(*ptr);
    } else if (**ptr == ')') {
      write_substr(&substr_arr, count, substr_beg_ptr, *ptr);
      (*ptr)++;
      break;
    } else {
      (*ptr)++;
    }
  }

  return substr_arr;
}

void free_substrs_arr(char **substrs, int substrs_count) {
  for (int i = 0; i < substrs_count; i++) {
    free(substrs[i]);
  }

  free(substrs);
}

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

ColumnDefinition *parse_table_definition(char **ptr, int *col_defs_count) {
  // find opening parenthesis
  while (**ptr != '(' && **ptr != '\0') {
    (*ptr)++;
  }

  if (**ptr == '\0') {
    printf("couldn't find opening parenthesis");
    return NULL;
  } else {
    (*ptr)++;
  }

  int substrs_count = 0;
  char **substrs = get_substrs(ptr, &substrs_count);
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

char *truncate_bounding_space(char *ptr) {
  // find start of word
  while (*ptr == ' ') {
    ptr++;
  }
  char *beg = ptr;
  // find end of word
  while (*ptr != ' ' && *ptr != '\0') {
    ptr++;
  }

  int res_len = ptr - beg + 1;
  char *res = malloc(res_len * sizeof(char));
  res[res_len - 1] = '\0';

  strncpy(res, beg, res_len - 1);

  return res;
}

char **parse_columns_to_insert(char **ptr, int *columns_count) {
  while (**ptr != '(' && **ptr != '\0') {
    (*ptr)++;
  }

  if (**ptr == '\0') {
    printf("couldn't find opening parenthesis\n");
    return NULL;
  } else {
    (*ptr)++;
  }

  *columns_count = 0;

  char **substrs = get_substrs(ptr, columns_count);
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
