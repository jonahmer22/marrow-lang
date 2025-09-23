#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../deps/ReMem/ReMem.h"

void error(int errorNumber, const char *message, bool quit);
uint64_t hashString(const char *str, size_t len);

#endif
