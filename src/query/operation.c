#include "../util/str_utils.h"
#include "types.h"
#include <stdio.h>

enum QueryOperation parse_oper_type(char **q_ptr) {
  char *buf_ptr = *q_ptr;

  while (**q_ptr != '\0' && **q_ptr != ' ') {
    (*q_ptr)++;
  }

  if (substrcmp(buf_ptr, *q_ptr, "CREATE")) {
    return Q_OPER_CREATE;
  } else if (substrcmp(buf_ptr, *q_ptr, "INSERT")) {
    return Q_OPER_INSERT;
  } else if (substrcmp(buf_ptr, *q_ptr, "SELECT")) {
    return Q_OPER_SELECT;
  } else {
    return -1;
    printf("failed define operation");
  }
}
