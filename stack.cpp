#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "akinator.h"
#include "colors.h"

static Status   StackRealloc(Stack* stack);
static Status   StackReallocUp(Stack* stack);
static Status   StackReallocDown(Stack* stack);
static void_sex Poison(Stack* stack);

//----------------------------------- CTOR ----------------------------------

StackError StackCtor(Stack* stack) {
    assert(stack != nullptr);

    if (stack == NULL) {
        stack->error = StackError_kNullPtr;
        return stack->error;
    }

    stack->number_of_elems = 0;
    stack->capacity = kDefaultCapacity;

    stack->data = (stack_t*)calloc((size_t)stack->capacity, sizeof(stack_t));

    if (stack->data == NULL) {
        stack->error = StackError_kMemoryAllocationError;
        return stack->error;
    }

    stack->error = StackError_kOk;

    return stack->error;
}

//----------------------------------- DTOR ----------------------------------

void_sex StackDtor(Stack* stack) {
    assert(stack != nullptr);
    StackAssert(stack);

    free(stack->data);

    // stack->number_of_elems =  0;
    // stack->capacity =         0;

    // stack->data =             NULL;
    // stack->error =       StackError_kOk;

    memset(stack, sizeof(stack), 0);
}

//--------------------------------- PUSH -------------------------------------

StackError StackPush(Stack* stack, stack_t new_elem) {
    StackAssert(stack);

    if (stack->number_of_elems + 1 == stack->capacity - 2) {
        stack->capacity *= kMulReallocUp;
        StackRealloc(stack);
    }

    stack->data[stack->number_of_elems + 1] = new_elem;
    stack->number_of_elems++;

    Poison(stack);

    StackAssert(stack);
    return StackError_kOk;
}

//--------------------------------- POP --------------------------------------

StackError StackPop(Stack* stack, stack_t* top_elem) {
    StackAssert(stack);

    if (stack->number_of_elems <= 0) {
        stack->error = StackError_kZeroElemes;
        return stack->error;
    }

    if (top_elem == NULL) {
        stack->error = StackError_kEmptyTopElem;
        return stack->error;
    }

    fprintf(stderr, "n of elems %ld\n", stack->number_of_elems);

    *top_elem = (stack->data)[stack->number_of_elems - 1];

    if (stack->number_of_elems - 1 == (stack->capacity / (2 * kMulReallocUp))) {
        stack->capacity = stack->capacity > kDefaultCapacity
                        ? stack->capacity / kMulReallocUp
                        : kDefaultCapacity;
        StackRealloc(stack);
    }

    stack->number_of_elems--;

    Poison(stack);

    return stack->error;
}

//----------------------------------- ASSERT FUNCTION -----------------------------

StackError StackAssert(Stack* stack) {
    stack->error = StackError_kOk;

    if (stack->data == NULL) {
        stack->error = StackError::StackError_kMemoryAllocationError;
        abort();
    }

    if (stack->capacity <= 0) {
        stack->error = StackError::StackError_kWrongCapacity;
        abort();
    }

    if (stack->number_of_elems > stack->capacity - 2) {
        stack->error = StackError::StackError_kOverflow;
        abort();
    }

    if (stack->number_of_elems < 0) {
        stack->error = StackError::StackError_kUnderflow;
        abort();
    }
}

//---------------------------------- POISON FUNCTION ---------------------------------

static void_sex Poison(Stack* stack) {
    StackAssert(stack);

    for (int i = 1; (stack->number_of_elems) + i < (stack->capacity - 1); i++) {
        (stack->data)[(stack->number_of_elems) + i] = 0xC0CA14E;
    }
}

//---------------------------------- REALLOC UP --------------------------------------

static Status StackReallocUp(Stack* stack) {
    assert(stack != NULL);

    stack->capacity *= kMulReallocUp;
    stack->data = (stack_t*)realloc(stack->data, (size_t)(stack->capacity) * sizeof(stack_t));

    if (stack->data == nullptr) {
        stack->error = StackError_kReallocationfailed;
        return FAILURE;
    }

    return SUCCESS;
}

//--------------------------------- REALLOC DOWN -------------------------------------
static Status StackReallocDown(Stack* stack) {
    assert(stack != nullptr);

    stack->capacity /= kMulReallocUp;
    stack->data = (stack_t*)realloc(stack->data, (size_t)(stack->capacity) * sizeof(stack_t));

    if (stack->data == nullptr) {
        stack->error = StackError_kReallocationfailed;
        return FAILURE;
    }

    return SUCCESS;
}

//============================= REALLOC IF NEEDED ===============================
static Status StackRealloc(Stack* stack) {
    assert(stack != nullptr);

    if (sizeof(stack->data)/sizeof(stack_t) < stack->capacity / kDivForReallocDown
     && stack->capacity > kDefaultCapacity) {
        if (StackReallocDown(stack) == FAILURE) {
            stack->error = StackError_kReallocationfailed;
            return FAILURE;
        }
    }

    if (sizeof(stack->data)/sizeof(stack_t) >= stack->capacity) {
        if (StackReallocUp(stack) == FAILURE) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

//-------------------------------------- STRING CONVERTER ---------------------------------

// написать функцию которая будет принимать стак элем т и распечатывает его или оформляет
// в строку и передавать в дамп указатель на функцию
//char* string_converter(Stack* stack) {

    // size_t size_of_arg = ;

    // char* unconverted_str = (char*)malloc(size_of_arg * 2 + 1);

    // if (unconverted_str == NULL) {
    //     return NULL;
    // }

    // switch (size_of_arg) {
    //     case sizeof(char):
    //         sprintf(unconverted_str, "%c", *(char*)ptr);
    //         break;
    //     case sizeof(short):
    //         sprintf(unconverted_str, "%hd", *(short*)ptr);
    //         break;
    //     case sizeof(int):
    //         sprintf(unconverted_str, "%d", *(int*)ptr);
    //         break;
    //     case sizeof(long):
    //         sprintf(unconverted_str, "%ld", *(long*)ptr);
    //         break;
    //     case sizeof(float):
    //         sprintf(unconverted_str, "%f", *(float*)ptr);
    //         break;
    //     case sizeof(double):
    //         sprintf(unconverted_str, "%f", *(double*)ptr);
    //         break;
    //     case sizeof(long double):
    //         sprintf(unconverted_str, "%Lf", *(long double*)ptr);
    //         break;
    //     default: ________;
    // }

    // return unconverted_str;
    //}