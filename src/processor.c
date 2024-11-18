#include "processor.h"
#include "query/create.h"
#include "query/feedback.h"
#include "query/operation.h"
#include "query/target.h"
#include "query/types.h"
#include "storage/table.h"

#include <stdio.h>
#include <stdlib.h>

void print_syntax_mistake_feedback(char *query, char *ptr);

int process_create(char **q_ptr);

int process_insert(char **q_ptr);

int process_select(char **q_ptr);

void process_query(char *query) {
  printf("QUERY PROCESSING START\n");

  char *ptr = query;
  enum QueryOperation q_oper = parse_oper_type(&ptr);
  if (q_oper == -1) {
    printf("failed to parse query operation\n");
    print_syntax_mistake_feedback(query, ptr);
    return;
  }

  int res = 0;
  switch (q_oper) {
  case Q_OPER_CREATE:
    printf("CREATE oper\n");
    res = process_create(&ptr);
    if (res < 0) {
      print_syntax_mistake_feedback(query, ptr);
    }
    break;
  case Q_OPER_INSERT:
    process_insert(&ptr);
    if (res < 0) {
      print_syntax_mistake_feedback(query, ptr);
    }
    break;
  case Q_OPER_SELECT:
    process_select(&ptr);
    if (res < 0) {
      print_syntax_mistake_feedback(query, ptr);
    }
    break;
  default:
    print_syntax_mistake_feedback(query, ptr);
    return;
  }

  printf("QUERY PROCESSING END\n");
}

void print_syntax_mistake_feedback(char *query, char *ptr) {
  char *feedback = generate_syntax_mistake_feedback(query, ptr);
  printf("%s", feedback);
  free(feedback);
}

int process_create(char **q_ptr) {
  char *target = parse_preworded_target(q_ptr, "TABLE");
  if (target == NULL) {
    return -1;
  }

  int column_defs_count = 0;
  ColumnDefinition *column_defs =
      parse_table_definition(q_ptr, &column_defs_count);

  int res = create_table(target, column_defs, column_defs_count);
  if (res < 0) {
    free(column_defs);
    return res;
  }

  free(column_defs);

  return 0;
}

int process_insert(char **q_ptr) {
  printf("query operation: INSERT not implemented\n");
  return 0;
}

int process_select(char **q_ptr) {
  printf("query operation: SELECT not implemented\n");
  return 0;
}
