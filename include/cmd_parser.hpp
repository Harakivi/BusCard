#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "Cli.hpp"

#define MAX_CMD 10
#define UINT8 (void *)1
#define UINT16 (void *)2
#define UINT32 (void *)3

extern Drivers::cmd_t cmd_list[];

void addCmd(Drivers::cmd_t cmd);

bool parseCmd(char *string);