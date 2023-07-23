#! /usr/bin/python
import os

# script to build Expr.java with all the classes for the Abstract Syntax Tree
base_name = "Expr"
rel_path = "com/craftinginterpreters/lox"
types = [
   "Binary : Expr left, Token operator, Expr right",
   "Grouping : Expr expression",
   "Literal : Object value",
   "Unary : Token operator, Expr right",
]


# define interface with visit method for each subclass
def define_visitor(file, types):
   file.write("\tinterface Visitor<R> {\n")
   for t in types:
      type_name = t.split(":")[0].strip()
      file.write(f"\t\tR visit{type_name}{base_name}({type_name} {base_name.lower()});\n")
   file.write("\t}\n")


# define subclasses
def define_type(file, class_name, fields):
   file.write(f"\n\tstatic class {class_name} extends {base_name} {{\n")

   # write constructor
   file.write(f"\t\t{class_name} ({fields}) {{\n")
   fields = fields.split(", ")
   for field in fields:
      name = field.split(" ")[1]
      file.write(f"\t\t\tthis.{name} = {name};\n")
   file.write("\t\t}\n\n")

   # write accept method for Visitor pattern
   file.write("\t\t@Override\n")
   file.write("\t\t<R> R accept(Visitor<R> visitor) {\n")
   file.write(f"\t\t\treturn visitor.visit{class_name}{base_name}(this);\n")
   file.write("\t\t}\n\n")

   for field in fields:
      file.write(f"\t\tfinal {field};\n")

   file.write("\t}\n")


# calculate absolute path for the file
script_dir = os.path.dirname(__file__).rstrip("/.")
file_path = os.path.join(script_dir, rel_path, f"{base_name}.java")

with open(file_path, "w") as f:
   # write the base class
   f.write("package com.craftinginterpreters.lox;\n\n")
   f.write("import java.util.List;\n\n")
   f.write(f"abstract class {base_name} {{\n")

   # implement Visitor pattern
   define_visitor(f, types)

   # write subclasses
   for t in types:
      class_name = t.split(":")[0].strip()
      fields = t.split(":")[1].strip()
      define_type(f, class_name, fields)

   # write base accept method
   f.write("\n\tabstract <R> R accept(Visitor<R> visitor);\n")

   f.write("}\n")

