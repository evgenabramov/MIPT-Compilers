#pragma once

namespace ast {

class PrintVisitor;
class SymbolTreeVisitor;

class Program;
class MainClass;
class MethodInvocation;

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
class ArrayAccessExpression;
class NewArrayExpression;
class ExpressionList;
class MethodInvocationExpression;
class ThisExpression;
class NewExpression;

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
class ScopeStatement;
class MethodInvocationStatement;

class Declaration;
class DeclarationList;
class VariableDeclaration;
class ClassDeclaration;
class MethodDeclaration;
class ClassDeclarationList;
class Formal;
class FormalList;
class ClassStorage;

class NamedEntity;
class NamedVariable;
class NamedArrayElement;

class Type;
class SimpleType;
class ArrayType;

class Symbol;
class ScopeLayer;
class ScopeLayerTree;

}