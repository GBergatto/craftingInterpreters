package com.craftinginterpreters.lox;

import java.util.List;

abstract class Smtm {
	interface Visitor<R> {
		R visitExpressionSmtm(Expression smtm);
		R visitPrintSmtm(Print smtm);
	}

	static class Expression extends Smtm {
		Expression (Expr expression) {
			this.expression = expression;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitExpressionSmtm(this);
		}

		final Expr expression;
	}

	static class Print extends Smtm {
		Print (Expr expression) {
			this.expression = expression;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitPrintSmtm(this);
		}

		final Expr expression;
	}

	abstract <R> R accept(Visitor<R> visitor);
}
