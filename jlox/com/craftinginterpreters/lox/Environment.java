package com.craftinginterpreters.lox;

import java.util.HashMap;
import java.util.Map;

class Environment {
	final Environment enclosing; // reference to parent scope
	private final Map<String, Object> values = new HashMap<>();

	Environment() { // constructor for global scope
		enclosing = null;
	}

	Environment(Environment enclosing) { // local scope
		this.enclosing = enclosing;
	}

	Object get(Token name) {
		if (values.containsKey(name.lexeme)) {
			return values.get(name.lexeme);
		}

		// recursively look inside the enclosing scope
		if (enclosing != null) return enclosing.get(name);

		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}

	void define(String name, Object value) {
		values.put(name, value);
	}

	void assign(Token name, Object value) {
		if (values.containsKey(name.lexeme)) {
			values.put(name.lexeme, value);
			return;
		}

		if (enclosing != null) {
			enclosing.assign(name, value);
			return;
		}

		throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
	}
}
