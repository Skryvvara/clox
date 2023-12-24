#include <stdio.h>
#include "common.h"
#include "debug.h"
#include "vm.h"
#include "compiler.h"

vm_t vm;

void reset_stack() {
    vm.stack_top = vm.stack;
}

void init_vm() {
    reset_stack();
}

void free_vm() {

}

void push(value_t value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

value_t pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

static interpret_result_t run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    } while(false);

    for (;;) {
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                value_t constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_ADD: {
                BINARY_OP(+);
                break;
            }
            case OP_SUBTRACT: {
                BINARY_OP(-);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(*);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(/);
                break;
            }
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_RETURN: {
                print_value(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#ifdef DEBUG_TRACE_EXECUTION
    printf("      ");
    for (value_t* slot = vm.stack; slot < vm.stack_top; slot++) {
        printf("[ ");
        print_value(*slot);
        printf(" ]");
    }
    printf("\n");
    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

interpret_result_t interpret(const char* source){
    compile(source);
    return INTERPRET_OK;
}

