package com.craftinginterpreters.lox;

class BreakException extends RuntimeException {
  BreakException() {
    super(null, null, false, false);
  }
}
