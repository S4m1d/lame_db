#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool substrcmp(char *start_ptr, char *end_ptr, const char *str2) {
  for (char *ptr1 = start_ptr, *ptr2 = str2; ptr1 < end_ptr; ptr1++, ptr2++) {
    if (*ptr1 != *ptr2) {
      return false;
    }
  }

  return true;
}

void truncate_nlc(char *str) {
  char *new_line_ch_pos = strchr(str, '\n');

  if (new_line_ch_pos != NULL) {
    *new_line_ch_pos = '\0';
  }
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

char **get_substrs(char **q_ptr, int *count, char end_c) {
  *count = 0;
  char **substr_arr;

  char *substr_beg_ptr = *q_ptr;
  while (**q_ptr != '\0') {
    if (**q_ptr == ',') {
      write_substr(&substr_arr, count, substr_beg_ptr, *q_ptr);
      substr_beg_ptr = ++(*q_ptr);
    } else if (**q_ptr == end_c) {
      write_substr(&substr_arr, count, substr_beg_ptr, *q_ptr);
      (*q_ptr)++;
      break;
    } else {
      (*q_ptr)++;
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
