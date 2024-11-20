#include "akinator.h"
#include "game_modes.h"
#include "stack.h"

#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "colors.h"
#include "str_cmp.h"


int main(const int argc, const char* argv[]) {
    assert(argv != nullptr);

    if (argc < 3) {
        fprintf(stderr, "%sError: not enough files for parsing.%s\n", BCKGR_RED, DEFAULT);
        return FAILURE;
    }

    const char* database_file = argv[1];

    Game(database_file);

    //ThrowError какой-нибудь если на дебаге

    return 0;
}