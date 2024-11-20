#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "colors.h"

static Status   StackRealloc(Stack* stack);
static Status   StackReallocUp(Stack* stack);
static Status   StackReallocDown(Stack* stack);

//----------------------------------- CTOR ----------------------------------
StackError StackCtor(Stack* stack, size_t size_of_elem) {
    assert(stack != nullptr);

    stack->number_of_elems = 0;
    stack->capacity = kDefaultCapacity;
    stack->size_of_elem = size_of_elem;

    stack->data = (stack_t*)calloc((size_t)stack->capacity, size_of_elem);

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

    stack->number_of_elems =  0;
    stack->capacity =         0;
    stack->size_of_elem =     0;

    stack->data =             NULL;
    stack->error =  StackError_kOk;

   // memset(stack, sizeof(stack), 0);
}

//--------------------------------- PUSH -------------------------------------
StackError StackPush(Stack* stack, const stack_t new_elem) {
    StackAssert(stack);

    if (stack->number_of_elems + 1 == stack->capacity) {
        if (StackRealloc(stack) != SUCCESS) {
            return stack->error;
        }
    }

    stack_t new_data = (char*)stack->data + (stack->number_of_elems * stack->size_of_elem);
    memcpy(new_data, new_elem, stack->size_of_elem);

    stack->number_of_elems++;

    StackAssert(stack);
    return StackError_kOk;
}

//--------------------------------- POP --------------------------------------
StackError StackPop(Stack* stack, stack_t* top_elem) {
    StackAssert(stack);

    if (stack->number_of_elems <= 0) {
        stack->error = StackError_kUnderflow;
        return stack->error;
    }

    if (top_elem == NULL) {
        stack->error = StackError_kEmptyTopElem;
        return stack->error;
    }

    stack->number_of_elems--;
    stack_t data = (char*)stack->data + (stack->number_of_elems * stack->size_of_elem);
    memcpy(top_elem, data, stack->size_of_elem);

    if (stack->number_of_elems <= stack->capacity / kDivForReallocDown
        && stack->capacity > kDefaultCapacity) {
        StackRealloc(stack);
    }

    return stack->error;
}

//----------------------------------- ASSERT FUNCTION -----------------------------
StackError StackAssert(Stack* stack) {
    stack->error = StackError_kOk;

    if (stack->data == nullptr || stack == nullptr) {
        stack->error = StackError_kMemoryAllocationError;
        return stack->error;
        abort();
    }

    if (stack->capacity <= 0 || stack->size_of_elem <= 0) {
        stack->error = StackError_kWrongCapacity;
        return stack->error;
        abort();
    }

    if (stack->number_of_elems > stack->capacity) {
        stack->error = StackError_kOverflow;
        return stack->error;
        abort();
    }

    return stack->error;
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

    if (stack->capacity - 1 < stack->capacity / kDivForReallocDown
     && stack->capacity > kDefaultCapacity) {
        if (StackReallocDown(stack) == FAILURE) {
            stack->error = StackError_kReallocationfailed;
            return FAILURE;
        }
    }

    if (stack->capacity + 1 >= stack->capacity) {
        if (StackReallocUp(stack) == FAILURE) {
            return FAILURE;
        }
    }

    return SUCCESS;
}