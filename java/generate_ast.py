#! /usr/bin/python
import os

# script to build Expr.java and Smtm.java with all the classes for the Abstract Syntax Tree
REL_PATH = "com/craftinginterpreters/lox"

# define interface with visit method for each subclass
def define_visitor(file, base_name, types):
   file.write("\tinterface Visitor<R> {\n")
   for t in types:
      type_name = t.split(":")[0].strip()
      file.write(f"\t\tR visit{type_name}{base_name}({type_name} {base_name.lower()});\n")
   file.write("\t}\n")


# define subclasses
def define_type(file, base_name, class_name, fields):
   file.write(f"\n\tstatic class {class_name} extends {base_name} {{\n")

   # write constructor
   file.write(f"\t\t{class_name}({fields}) {{\n")
   fields = fields.split(", ")
   for field in fields:
      if field != "":
         name = field.split(" ")[1]
         file.write(f"\t\t\tthis.{name} = {name};\n")
   file.write("\t\t}\n\n")

   # write accept method for Visitor pattern
   file.write("\t\t@Override\n")
   file.write("\t\t<R> R accept(Visitor<R> visitor) {\n")
   file.write(f"\t\t\treturn visitor.visit{class_name}{base_name}(this);\n")
   file.write("\t\t}\n\n")

   for field in fields:
      if field != "":
         file.write(f"\t\tfinal {field};\n")

   file.write("\t}\n")


def generate(base_name, types):
   # calculate absolute path for the file
   script_dir = os.path.dirname(__file__).rstrip("/.")
   file_path = os.path.join(script_dir, REL_PATH, f"{base_name}.java")

   with open(file_path, "w") as f:
      # write the base class
      f.write("package com.craftinginterpreters.lox;\n\n")
      f.write("import java.util.List;\n\n")
      f.write(f"abstract class {base_name} {{\n")

      # implement Visitor pattern
      define_visitor(f, base_name, types)

      # write subclasses
      for t in types:
         class_name = t.split(":")[0].strip()
         fields = t.split(":")[1].strip()
         define_type(f, base_name, class_name, fields)

      # write base accept method
      f.write("\n\tabstract <R> R accept(Visitor<R> visitor);\n")

      f.write("}\n")


expr_types = [
   "Assign : Token name, Expr value",
   "Ternary: Expr condition, Expr thenBranch, Expr elseBranch",
   "Binary : Expr left, Token operator, Expr right",
   "Call : Expr callee, Token paren, List<Expr> arguments",
   "Get : Expr object, Token name",
   "Grouping : Expr expression",
   "Literal : Object value",
   "Logical : Expr left, Token operator, Expr right",
   "Set : Expr object, Token name, Expr value",
   "This : Token keyword",
   "Unary : Token operator, Expr right",
   "Variable : Token name",
]
stmt_types = [
   "Block : List<Stmt> statements",
   "Break : Token keyword",
   "Class : Token name, List<Stmt.Function> methods",
   "Continue : Token keyword",
   "Expression : Expr expression",
   "Function : Token name, List<Token> params, List<Stmt> body",
   "If : Expr condition, Stmt thenBranch, Stmt elseBranch",
   "Print : Expr expression",
   "Return : Token keyword, Expr value",
   "Var : Token name, Expr initializer",
   "While : Expr condition, Stmt body",
]

generate("Expr", expr_types)
generate("Stmt", stmt_types)
