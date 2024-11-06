#pragma once

#define MAX_QUERY_SIZE 100

enum QueryOperation { Q_OPER_CREATE, Q_OPER_INSERT, Q_OPER_SELECT };

enum QueryOperation parse_oper_type(char **ptr);

enum DataType {
  DT_INT,
  DT_TEXT,
};

#define COL_NAME_LENGTH_MAX 51

typedef struct {
  enum DataType t;
  char name[COL_NAME_LENGTH_MAX];
} ColumnDefinition;

char *parse_target(char **ptr, enum QueryOperation q_oper);

ColumnDefinition *parse_table_definition(char **ptr, int *col_defs_count);
