#pragma once

enum QueryOperation { Q_OPER_CREATE, Q_OPER_INSERT, Q_OPER_SELECT };

enum DataType {
  DT_INT,
  DT_TEXT,
};

#define COL_NAME_LENGTH_MAX 51

typedef struct {
  enum DataType t;
  char name[COL_NAME_LENGTH_MAX];
} ColumnDefinition;

