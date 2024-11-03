#pragma once

#define MAX_QUERY_SIZE 50

enum QueryOperation { Q_OPER_CREATE, Q_OPER_INSERT, Q_OPER_SELECT };

enum QueryOperation parse_oper_type(char **ptr);

enum ColumnType {
  INT,
  STRING,
};

typedef struct {
  enum ColumnType t;
  char *name;
} ColumnTypeName;

char *parse_target(char **ptr, enum QueryOperation q_oper);
