#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#include <cstdlib>
#include <cstdint>

#include "colors.h"
#include "str_cmp.h"

static const uint32_t kMaxStringSize = 128;

typedef void   void_sex;
typedef char*  elem_t;

typedef enum Answers {
    GUESS =            0x00,
    DEFINE =           0x01,
    SHOW_TREE =        0x02,
    COMPARE =          0x03,

    EXIT =             0x04,
    CONTINUE =         0x05,

    YES =              0x06,
    NO =               0x07,

    MENU =             0x08,

    UNKNOWN_COMMAND = -0x01
} Answers;

typedef enum AkinatorError {
    AkinatorError_kOk =                    0x00,

    AkinatorError_kParsingError =          0x01,
    AkinatorError_kMemoryAllocationError = 0x02,
    AkinatorError_kReadingFromFileError =  0x03
} AkinatorError;

typedef enum Status {
    SUCCESS = 0x00,
    FAILURE = 0x01
} Status;

typedef struct node_t {
    node_t* parent;

    elem_t  data;

    node_t* right;
    node_t* left;
} node_t;

typedef struct Akinator {
    AkinatorError error;

    node_t*       last_node;
    //еще есть идея хранить здесь указател на корень дерева чтобы вызыать тупо сруктуру
    //стэк для записи шагов и для попа при распечатке определения
} Akinator;

//---------------------------- PUBLIC ----------------------------
Akinator*  AkinatorCtor();
Status     AkinatorDtor(Akinator* akinator);
Status     TreeDtor(node_t* root);

node_t*    GetTreeFromDataBaseFile(const char* name_of_database_file, Akinator* akinator);

void_sex   InsertNewNode(node_t* root, elem_t new_data, elem_t new_question, node_t* previous_data);

Answers    GetAnswer();

void_sex   ReportError(const char* message);

void_sex   PrintDisneyGenie();
void_sex   PrintSleepingGenie() ;

//NOTE в плюсах же есть способ лишить пользователя возможности вызова данных функций в мэйне
//при этом оставить возможным вызывать эти функции как подфункции?????????????


#endif //AKINATOR_H_