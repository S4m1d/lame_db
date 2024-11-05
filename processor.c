#include "processor.h"
#include "query.h"
#include <stdio.h>
#include <stdlib.h>

void process_query(char *query) {
  char *ptr = query;
  enum QueryOperation q_oper = parse_oper_type(&ptr);
  if (q_oper == -1) {
    printf("failed to parse query operation\n");
    return;
  }

  switch (q_oper) {
  case Q_OPER_CREATE:
    printf("query operation: CREATE\n");
    break;
  case Q_OPER_INSERT:
    printf("query operation: INSERT\n");
    break;
  case Q_OPER_SELECT:
    printf("query operation: SELECT\n");
    break;
  default:
    printf("query operation: invalid state\n");
    return;
  }

  char *target = parse_target(&ptr, q_oper);
  if (target == NULL) {
    printf("query target not found\n");
    return;
  }

  printf("query target: %s\n", target);

  free(target);

  if (q_oper == Q_OPER_CREATE) {
    // TODO: collect results
    parse_table_definition(&ptr);
  }
}
