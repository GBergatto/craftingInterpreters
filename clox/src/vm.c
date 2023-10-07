#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

// the author uses a single global VM object to keep the code lighter
// however, passing around a VM pointer would give the host more flexibility
VM vm;

static void resetStack() {
  vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
}

// add one element to the stack
void push(Value value) {
  // store value in the cell pointed by stackTop
  *vm.stackTop = value;
  vm.stackTop++;
}

// remove one element from the stack
Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
  do { \
    double b = pop(); \
    double a = pop(); \
    push(a op b); \
  } while (false) // ensures all statements are in a single scope

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    // stack tracing
    printf(" ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");

    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      // decode the instruction
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_ADD: BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE:   BINARY_OP(/); break;
      #undef BINARY_OP
      case OP_NEGATE: push(-pop()); break;
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

  #undef READ_BYTE
  #undef READ_CONSTANT
  #undef BINARY_OP
}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}
