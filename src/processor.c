#include "processor.h"
#include "query/create.h"
#include "query/feedback.h"
#include "query/operation.h"
#include "query/target.h"

#include <stdio.h>
#include <stdlib.h>
void print_syntax_mistake_feedback(char *query, char *ptr);

void process_query(char *query) {
  char *ptr = query;
  enum QueryOperation q_oper = parse_oper_type(&ptr);
  if (q_oper == -1) {
    printf("failed to parse query operation\n");
    print_syntax_mistake_feedback(query, ptr);
    return;
  }

  // TODO: remove after debug
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
    print_syntax_mistake_feedback(query, ptr);
    return;
  }

  printf("query target: %s\n", target);

  free(target);

  if (q_oper == Q_OPER_CREATE) {
    int column_defs_count = 0;
    ColumnDefinition *column_defs =
        parse_table_definition(&ptr, &column_defs_count);

    free(column_defs);
  } else if (q_oper == Q_OPER_INSERT) {
  } else if (q_oper == Q_OPER_SELECT) {
  }
}

void print_syntax_mistake_feedback(char *query, char *ptr) {
  char *feedback = generate_syntax_mistake_feedback(query, ptr);
  printf("%s", feedback);
  free(feedback);
}
