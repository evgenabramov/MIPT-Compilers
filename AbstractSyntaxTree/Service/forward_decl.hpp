#pragma once

namespace ast {

class PrintVisitor;
class Interpreter;

class Program;
class MainClass;

class Expression;
class AddExpression;
class AndExpression;
class BoolExpression;
class DivExpression;
class GreaterEqualExpression;
class GreaterExpression;
class IdentExpression;
class IntExpression;
class LessEqualExpression;
class LessExpression;
class MulExpression;
class NotExpression;
class OrExpression;
class SubExpression;
class ModExpression;
class EqualExpression;

class Statement;
class StatementList;
class AssignmentStatement;
class IfStatement;
class PrintStatement;
class ReturnStatement;
class VariableDeclarationStatement;
class WhileStatement;
class AssertStatement;
class IfElseStatement;

class Declaration;
class DeclarationList;
class VariableDeclaration;

class NamedEntity;
class NamedVariable;

class Type;
class SimpleType;
class ArrayType;

class Value;
class IntValue;
class BoolValue;

}