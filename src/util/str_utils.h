#include <stdbool.h>

void skip_spaces(char **q_ptr) ;

bool substrcmp(const char *start_ptr, const char *end_ptr, const char *str2);

void truncate_nlc(char *);

char *truncate_bounding_space(char *ptr);

char **get_substrs(char **q_ptr, int *count, char delim, char end_c);

void free_substrs_arr(char **substrs, int substrs_count);

int skip_word(char **dest, const char *word);

char *find_first(char *str, char c);
