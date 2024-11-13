#pragma once

#include "limits.h"

enum DataType {
  DT_INT,
  DT_TEXT,
};

typedef struct {
  enum DataType t;
  char name[COL_NAME_LENGTH_MAX];
} ColumnDefinition;
