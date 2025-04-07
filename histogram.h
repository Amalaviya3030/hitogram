#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char buffer[BUFFER_SIZE];
    int read_index;
    int write_index;
} shared_memory_t;
