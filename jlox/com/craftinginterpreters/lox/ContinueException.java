package com.craftinginterpreters.lox;

class ContinueException extends RuntimeException {
  ContinueException() {
    super("Continue exception");
  }
}

