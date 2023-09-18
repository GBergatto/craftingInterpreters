package com.craftinginterpreters.lox;

import java.util.List;

class LoxFunction implements LoxCallable {  
	private final String name;
	private final List<Token> params;
	private final List<Stmt> body;
	private final Environment closure;
	private final boolean isInitializer;

	LoxFunction(String name, List<Token> params, List<Stmt> body, Environment closure, boolean isInitializer) {
		this.name = name;
		this.closure = closure;
		this.params = params;
		this.body = body;
		this.isInitializer = isInitializer;
	}

	// create a new enviroment and bind 'this' to the object's instance
	LoxFunction bind(LoxInstance instance) {
		Environment environment = new Environment(closure);
		environment.define("this", instance);
		return new LoxFunction(name, params, body, environment, isInitializer);
	}

	@Override
	public int arity() {
		return params.size();
	}

	@Override
	public Object call(Interpreter interpreter, List<Object> arguments) {
		// define all paramethers inside a new scope and assign values passed as arguments
		Environment environment = new Environment(closure);
		for (int i = 0; i < params.size(); i++) {
			environment.define(params.get(i).lexeme, arguments.get(i));
		}

		try {
			interpreter.executeBlock(body, environment);
		} catch (ReturnException returnException) {
			if (isInitializer) return closure.getAt(0, "this");
			return returnException.value;
		}

		if (isInitializer) { // initializers always return "this"
			return closure.getAt(0, "this");
		}
		return null;
	}

	@Override
	public String toString() {
		return "<fn " + name+ ">";
	}
}
