package com.craftinginterpreters.lox;

import java.util.List;

class LoxFunction implements LoxCallable {  
	private final Stmt.Function declaration;
	private final Environment closure;

	LoxFunction(Stmt.Function declaration, Environment closure) {
		this.closure = closure;
		this.declaration = declaration;
	}

	// create a new enviroment and bind 'this' to the object's instance
	LoxFunction bind(LoxInstance instance) {
		Environment environment = new Environment(closure);
		environment.define("this", instance);
		return new LoxFunction(declaration, environment);
	}

	@Override
	public int arity() {
		return declaration.params.size();
	}

	@Override
	public Object call(Interpreter interpreter, List<Object> arguments) {
		// define all paramethers inside a new scope and assign values passed as arguments
		Environment environment = new Environment(closure);
		for (int i = 0; i < declaration.params.size(); i++) {
			environment.define(declaration.params.get(i).lexeme, arguments.get(i));
		}

		try {
			interpreter.executeBlock(declaration.body, environment);
		} catch (ReturnException returnException) {
			return returnException.value;
		}
		return null;
	}

	@Override
	public String toString() {
		return "<fn " + declaration.name.lexeme + ">";
	}
}
