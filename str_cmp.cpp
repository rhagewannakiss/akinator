#include "str_cmp.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <search.h>

int str_cmp(const char* str1, const char* str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int str1_index = 0;
    int str2_index = 0;

    while (!isalpha(str1[str1_index])
        && (str1_index < (int)strlen(str1))) {
        fprintf(stderr, "index symb %d  %d \n", str1[str1_index], str1_index);
        str1_index++;
    }
    while (!isalpha(str2[str2_index])
        && (str2_index < (int)strlen(str2))) {
        fprintf(stderr, "index symb %d  %d \n", str2[str2_index], str2_index);
        str2_index++;
    }

    while ((tolower(str1[str1_index]) != '\0')
        && (tolower(str2[str2_index]) != '\0')) {
        if (tolower(str1[str1_index]) != tolower(str2[str2_index])) {
            return  tolower(str1[str1_index]) - tolower(str2[str2_index]);
        }
        str1_index++;
        str2_index++;
    }
    if (strlen(str1) != strlen(str2)) {
        return (int)strlen(str1) - (int)strlen(str2);
    }

    return 0;
}