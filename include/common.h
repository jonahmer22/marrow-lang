#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>

#include "../deps/ReMem/ReMem.h"

void error(int errorNumber, const char *message, bool quit);

#endif
