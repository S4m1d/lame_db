#pragma once

#include "hash_table/table.h"

#define STORAGE_PATH "storage"

#define TBLS_HASH_FUNC_MODULUS 11

#define COL_HASH_FUNC_MODULUS 11

TablesHashTable *scan_tables();
