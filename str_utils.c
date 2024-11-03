#include <stdbool.h>

bool substrcmp(char *start_ptr, char *end_ptr, const char *str2) {
  for (char *ptr1 = start_ptr, *ptr2 = str2; ptr1 < end_ptr; ptr1++, ptr2++) {
    if (*ptr1 != *ptr2) {
      return false;
    }
  }

  return true;
}
