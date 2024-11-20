#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>

#include "akinator.h"


static const int kDefaultCapacity =   32;
static const int kMulReallocUp =      2;
static const int kDivForReallocDown = 4;

typedef void  void_sex;
typedef void* stack_t;

typedef enum StackError {
    StackError_kOk =                    0x00,

    StackError_kMemoryAllocationError = 0x01,
    StackError_kReallocationfailed =    0x02,
    StackError_kNullPtr =               0x03,

    StackError_kUnderflow  =            0x04,
    StackError_kOverflow =              0x05,

    StackError_kWrongCapacity =         0x06,
    StackError_kZeroElemes =            0x07,

    StackError_kEmptyTopElem =          0x08,
} StackError;

typedef struct Stack {
    stack_t*   data;

    size_t    number_of_elems;
    size_t    capacity;
    size_t     size_of_elem;

    StackError error;
} Stack;

//*---------------------------------- PUBLIC ----------------------------------
StackError  StackCtor(Stack* stack, size_t size_of_elem);
void_sex    StackDtor(Stack* stack);

StackError  StackPush(Stack* stack, stack_t new_elem);
StackError  StackPop(Stack* stack, stack_t* top_elem);

StackError  StackAssert(Stack* stack);

#endif // STACK_H_