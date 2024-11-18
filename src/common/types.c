#include "types.h"

const int DT_INT_SIZE = sizeof(int);

const int DT_TEXT_SIZE = sizeof(char) * 50;

int dt_sizes[] = {
    DT_INT_SIZE,
    DT_TEXT_SIZE,
};

char *dt_names[] = {
    "INT",
    "TEXT",
};
