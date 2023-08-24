package com.craftinginterpreters.lox;

import java.util.HashMap;
import java.util.Map;

class LoxInstance {
  private LoxClass klass;
  private final Map<String, Object> fields = new HashMap<>();

  LoxInstance(LoxClass klass) {
    this.klass = klass;
  }

  @Override
  public String toString() {
    return klass.name + " instance";
  }

  Object get(Token name) {
    // look for a field
    if (fields.containsKey(name.lexeme)) {
      return fields.get(name.lexeme);
    }

    // look for a method in the instance's class
    LoxFunction method = klass.findMethod(name.lexeme);
    if (method != null) return method.bind(this);

    // throw an error if no property with this name is found
    throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
  }

  void set(Token name, Object value) {
    fields.put(name.lexeme, value);
  }
}
