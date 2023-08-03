package com.craftinginterpreters.lox;

class BreakException extends RuntimeException {
  BreakException() {
    super("Break exception");
  }
}
