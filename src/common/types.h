#pragma once

enum DataType {
  DT_INT,
  DT_TEXT,
};

#define COL_NAME_LENGTH_MAX 51

typedef struct {
  enum DataType t;
  char name[COL_NAME_LENGTH_MAX];
} ColumnDefinition;
