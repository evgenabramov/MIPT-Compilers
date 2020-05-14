#pragma once

#include "Service/TreeNode.hpp"
#include "Service/Program.hpp"
#include "Service/MainClass.hpp"

#include "Expression/Expression.hpp"
#include "Expression/AddExpression.hpp"
#include "Expression/AndExpression.hpp"
#include "Expression/BoolExpression.hpp"
#include "Expression/DivExpression.hpp"
#include "Expression/GreaterEqualExpression.hpp"
#include "Expression/GreaterExpression.hpp"
#include "Expression/IdentExpression.hpp"
#include "Expression/IntExpression.hpp"
#include "Expression/LessEqualExpression.hpp"
#include "Expression/LessExpression.hpp"
#include "Expression/MulExpression.hpp"
#include "Expression/NotExpression.hpp"
#include "Expression/OrExpression.hpp"
#include "Expression/SubExpression.hpp"
#include "Expression/ModExpression.hpp"
#include "Expression/EqualExpression.hpp"
#include "Expression/ArrayAccessExpression.hpp"

#include "Declaration/Declaration.hpp"
#include "Declaration/DeclarationList.hpp"
#include "Declaration/VariableDeclaration.hpp"
#include "Declaration/ArrayDeclaration.hpp"

#include "Statement/Statement.hpp"
#include "Statement/StatementList.hpp"
#include "Statement/AssignmentStatement.hpp"
#include "Statement/IfStatement.hpp"
#include "Statement/PrintStatement.hpp"
#include "Statement/ReturnStatement.hpp"
#include "Statement/VariableDeclarationStatement.hpp"
#include "Statement/WhileStatement.hpp"
#include "Statement/AssertStatement.hpp"
#include "Statement/IfElseStatement.hpp"
#include "Statement/ArrayDeclarationStatement.hpp"
#include "Statement/ArrayAssignmentStatement.hpp"
#include "Statement/ArrayElementAssignmentStatement.hpp"

#include "NamedEntity/NamedEntity.hpp"
#include "NamedEntity/NamedVariable.hpp"
#include "NamedEntity/NamedArray.hpp"

#include "Types/Type.hpp"
#include "Types/SimpleType.hpp"
#include "Types/ArrayType.hpp"

#include "Visitors/InterpreterValues/Value.hpp"
#include "Visitors/InterpreterValues/IntValue.hpp"
#include "Visitors/InterpreterValues/BoolValue.hpp"