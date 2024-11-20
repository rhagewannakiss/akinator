#ifndef GAME_MODES_H_
#define GAME_MODES_H_

#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "akinator.h"
#include "stack.h"
#include "colors.h"
#include "str_cmp.h"

//---------------------------- PUBLIC ----------------------------
void_sex Game(const char* database_file);

#endif //GAME_MODES_H_