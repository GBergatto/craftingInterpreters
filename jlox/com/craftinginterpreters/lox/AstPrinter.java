package com.craftinginterpreters.lox;

import java.util.List;

class AstPrinter implements Expr.Visitor<String>, Stmt.Visitor<String> {
	// visit the first (outer) expression
	String print(Expr expr) {
		return expr.accept(this);
	}

	// override the visit method of each expression type
	@Override
	public String visitBinaryExpr(Expr.Binary expr) {
		return parenthesize(expr.operator.lexeme, expr.left, expr.right);
	}

	@Override
	public String visitGroupingExpr(Expr.Grouping expr) {
		return parenthesize("group", expr.expression);
	}

	@Override
	public String visitLiteralExpr(Expr.Literal expr) {
		if (expr.value == null) return "nil";
		return expr.value.toString();
	}

	@Override
	public String visitUnaryExpr(Expr.Unary expr) {
		return parenthesize(expr.operator.lexeme, expr.right);
	}

	@Override
	public String visitVariableExpr(Expr.Variable expr) {
		return expr.name.toString();
	}

	@Override
	public String visitAssignExpr(Expr.Assign expr) {
		return parenthesize2("=", expr.name.lexeme, expr.value);
	}

	@Override
	public String visitExpressionStmt(Stmt.Expression stmt) {
		return parenthesize(";", stmt.expression);
	}

	@Override
	public String visitVarStmt(Stmt.Var stmt) {
		if (stmt.initializer == null) {
			return parenthesize2("var", stmt.name);
		}

		return parenthesize2("var", stmt.name, "=", stmt.initializer);
	}

	@Override
	public String visitPrintStmt(Stmt.Print stmt) {
		return parenthesize("print", stmt.expression);
	}

	private String parenthesize(String name, Expr... exprs) {
		StringBuilder builder = new StringBuilder();
		builder.append("(").append(name);
		for (Expr expr : exprs) {
			builder.append(" ");
			builder.append(expr.accept(this)); // visit recursively the subexpression
		}
		builder.append(")");
		return builder.toString();
	}

	private String parenthesize2(String name, Object... parts) {
		StringBuilder builder = new StringBuilder();

		builder.append("(").append(name);
		transform(builder, parts);
		builder.append(")");

		return builder.toString();
	}

	private void transform(StringBuilder builder, Object... parts) {
		for (Object part : parts) {
			builder.append(" ");
		if (part instanceof Expr) {
				builder.append(((Expr) part).accept(this));
			} else if (part instanceof Stmt) {
				builder.append(((Stmt) part).accept(this));
			} else if (part instanceof Token) {
				builder.append(((Token) part).lexeme);
			} else if (part instanceof List) {
				transform(builder, ((List) part).toArray());
			} else {
				builder.append(part);
			}
		}
	}
}
