#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/types.h"
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

  char **substrs = get_substrs(q_ptr, columns_count, ',', ')');
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
int count_vals_sets(char **ptr);

void free_val_sets(void ***val_sets, int val_sets_count, int columns_count);

void **parse_values_set(char **q_ptr, ColumnDefinition *column,
                        int columns_count);

void ***parse_values_to_insert(char **q_ptr, ColumnDefinition *column_defs,
                               int columns_count, int *val_sets_count) {

  int res = skip_word(q_ptr, "VALUES");
  if (res != 0) {
    return NULL;
  }

  char *ptr = *q_ptr;

  *val_sets_count = 0;
  *val_sets_count = count_vals_sets(&ptr);

  if (*val_sets_count < 0) {
    // set error position
    *q_ptr = ptr;
    return NULL;
  } else if (*val_sets_count == 0) {
    printf("no values sets provided");
    return NULL;
  }

  void ***val_sets = malloc(*val_sets_count * sizeof(void **));

  for (int i = 0; i < *val_sets_count; i++) {
    skip_spaces(q_ptr);

    void **val_set = parse_values_set(q_ptr, column_defs, columns_count);
    if (val_set == NULL) {
      free_val_sets(val_sets, *val_sets_count, columns_count);
      return NULL;
    }

    val_sets[i] = val_set;
  }

  return val_sets;
}

void free_val_sets(void ***val_sets, int val_sets_count, int columns_count) {
  for (int i = 0; i < val_sets_count; i++) {
    for (int j = 0; j < columns_count; j++) {
      free(val_sets[i][j]);
    }
    free(val_sets[i]);
  }

  free(val_sets);
}

int count_vals_sets(char **ptr) {
  // 0 -- opening parenthesis, 1 -- closing one
  int expect = 0;
  int count = 0;
  while (**ptr != '\0') {
    if (**ptr == '(') {

      if (expect == 0) {
        expect = 1;
      } else {
        printf("expected closing parenthesis");

        return -1;
      }

    } else if (**ptr == ')') {

      if (expect == 1) {
        count++;

        expect = 0;
      } else {
        printf("expected opening parenthesis");

        return -1;
      }
    }
    (*ptr)++;
  }

  if (expect != 0) {
    printf("last values set is not closed with parenthesis");
    return -1;
  }

  return count;
}

void *parse_text_value(char *str, int size);
void *parse_int_value(char *str, int size);

void *(*value_parsers[])(char *, int) = {
    parse_int_value,
    parse_text_value,
};

const int DT_INT_SIZE = sizeof(int);

const int DT_TEXT_SIZE = sizeof(char) * 50;

int dt_sizes[] = {
    DT_INT_SIZE,
    DT_TEXT_SIZE,
};

void **parse_values_set(char **q_ptr, ColumnDefinition *column_defs,
                        int columns_count) {
  if (**q_ptr != '(') {
    printf("couldn't find opening parenthesis of values set");
    return NULL;
  }

  (*q_ptr)++;

  int substrs_count = 0;

  char **substrs = get_substrs(q_ptr, &substrs_count, ',', ')');
  if (substrs == NULL) {
    printf("failed parse values set");
    return NULL;
  }

  if (substrs_count != columns_count) {
    printf("columns=%d while values=%d", columns_count, substrs_count);
    return NULL;
  }

  void **vals = malloc(columns_count * sizeof(void *));

  for (int i = 0; i < substrs_count; i++) {
    char *truncated_substr = truncate_bounding_space(substrs[i]);

    ColumnDefinition column_def = column_defs[i];
    void *val =
        value_parsers[column_def.t](truncated_substr, dt_sizes[column_def.t]);

    vals[i] = val;

    free(truncated_substr);
  }

  free_substrs_arr(substrs, substrs_count);

  return vals;
}

void *parse_text_value(char *str, int size) {
  if (*str != '\'') {
    printf("text value must be bound in quotes");
    return NULL;
  }

  str++;

  char *start_ptr = str;

  while (*str != '\'' && *str != '\0') {
    str++;
  }

  int len = str - start_ptr;

  if (len > size) {
    printf("text must be <= %d long", size);

    return NULL;
  }

  char *val = malloc(size + 1 * sizeof(char));

  strncpy(val, start_ptr, len);
  val[len] = '\0';

  return val;
}

void *parse_int_value(char *str, int size) {
  char *end_ptr;
  long val_long = strtol(str, &end_ptr, 10);

  errno = 0;
  if (errno != 0 || end_ptr == str) {
    printf("convertion failed");
    return NULL;
  }

  if (val_long < INT_MIN || val_long > INT_MAX) {
    printf("%ld is out of int bounds", val_long);
    return NULL;
  }

  int *val = malloc(size);
  if (val == NULL) {
    printf("failed to allocate memory for int value");
    return NULL;
  }

  *val = (int)val_long;

  return val;
}
