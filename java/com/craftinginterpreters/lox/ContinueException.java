package com.craftinginterpreters.lox;

class ContinueException extends RuntimeException {
  ContinueException() {
    super(null, null, false, false);
  }
}

