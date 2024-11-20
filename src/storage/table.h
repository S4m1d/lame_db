#pragma once

#include "../common/types.h"

int create_table(const char *table_name, ColumnDefinition *column_defs,
                 int columns_count);

PosColumnDefinition **read_table_definition(const char *table_name,
                                        int *columns_count);
