#pragma once

#include "limits.h"

enum DataType {
  DT_INT,
  DT_TEXT,
};


extern int dt_sizes[];

extern char *dt_names[];

typedef struct {
  enum DataType t;
  char name[COL_NAME_LEN_MAX];
} ColumnDefinition;
