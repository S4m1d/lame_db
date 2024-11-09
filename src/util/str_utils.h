#include <stdbool.h>

bool substrcmp(char *start_ptr, char *end_ptr, const char *str2);

void truncate_nlc(char *);

char *truncate_bounding_space(char *ptr);

char **get_substrs(char **q_ptr, int *count, char end_c);

void free_substrs_arr(char **substrs, int substrs_count);
