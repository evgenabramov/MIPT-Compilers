cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0076 NEW)

set(
        PROJECT_NAME
        Compiler
)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
        HEADERS
        ${CMAKE_SOURCE_DIR}/Grammar/driver.hh
        ${CMAKE_SOURCE_DIR}/Grammar/parser.hh
)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(
        MyParser
        ${CMAKE_SOURCE_DIR}/Grammar/parser.y
        ${CMAKE_SOURCE_DIR}/Grammar/parser.cpp
        DEFINES_FILE ${CMAKE_SOURCE_DIR}/Grammar/parser.hh
        COMPILE_FLAGS --graph
        VERBOSE ${CMAKE_SOURCE_DIR}/Grammar/found_conflicts.txt
)

FLEX_TARGET(
        MyScanner
        ${CMAKE_SOURCE_DIR}/Grammar/scanner.l
        ${CMAKE_SOURCE_DIR}/Grammar/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        Grammar/driver.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}
        Grammar/Service
        Visitors
        Grammar
        Grammar/Expression
        Grammar/Declaration
        Grammar/Statement
        Grammar/NamedEntity
        Grammar/Type
        SymbolTable
        Objects
        MemberType
        MethodMechanisms
)

target_sources(
        ${PROJECT_NAME}
        PUBLIC

        Grammar/Expression/Expression.hpp
        Grammar/Expression/AddExpression.hpp
        Grammar/Expression/AndExpression.hpp
        Grammar/Expression/BoolExpression.hpp
        Grammar/Expression/DivExpression.hpp
        Grammar/Expression/GreaterEqualExpression.hpp
        Grammar/Expression/GreaterExpression.hpp
        Grammar/Expression/IdentExpression.hpp
        Grammar/Expression/IntExpression.hpp
        Grammar/Expression/LessEqualExpression.hpp
        Grammar/Expression/LessExpression.hpp
        Grammar/Expression/MulExpression.hpp
        Grammar/Expression/NotExpression.hpp
        Grammar/Expression/OrExpression.hpp
        Grammar/Expression/SubExpression.hpp
        Grammar/Expression/ModExpression.hpp
        Grammar/Expression/EqualExpression.hpp
        Grammar/Expression/ArrayAccessExpression.hpp
        Grammar/Expression/NewArrayExpression.hpp
        Grammar/Expression/ExpressionList.hpp
        Grammar/Expression/MethodInvocationExpression.hpp
        Grammar/Expression/ThisExpression.hpp
        Grammar/Expression/NewExpression.hpp
        Grammar/Expression/ArrayLengthExpression.hpp

        Grammar/Declaration/Declaration.hpp
        Grammar/Declaration/DeclarationList.hpp
        Grammar/Declaration/VariableDeclaration.hpp
        Grammar/Declaration/ClassDeclaration.hpp
        Grammar/Declaration/MethodDeclaration.hpp
        Grammar/Declaration/ClassDeclarationList.hpp
        Grammar/Declaration/Formal.hpp
        Grammar/Declaration/FormalList.hpp

        Grammar/Statement/Statement.hpp
        Grammar/Statement/StatementList.hpp
        Grammar/Statement/AssignmentStatement.hpp
        Grammar/Statement/IfStatement.hpp
        Grammar/Statement/PrintStatement.hpp
        Grammar/Statement/ReturnStatement.hpp
        Grammar/Statement/VariableDeclarationStatement.hpp
        Grammar/Statement/WhileStatement.hpp
        Grammar/Statement/AssertStatement.hpp
        Grammar/Statement/IfElseStatement.hpp
        Grammar/Statement/ScopeStatement.hpp
        Grammar/Statement/MethodInvocationStatement.hpp

        Grammar/Service/TreeNode.hpp
        Grammar/Service/MainClass.hpp
        Grammar/Service/Program.hpp
        Grammar/Service/forward_decl.hpp
        Grammar/Service/MethodInvocation.hpp

        Visitors/Visitor.hpp
        Visitors/include_list.hpp
        Visitors/PrintVisitor.hpp
        Visitors/SymbolTreeVisitor.hpp
        Visitors/MethodCallVisitor.cpp
        Visitors/TemplateVisitor.hpp

        Grammar/NamedEntity/NamedEntity.hpp
        Grammar/NamedEntity/NamedVariable.hpp
        Grammar/NamedEntity/NamedArrayElement.hpp

        Grammar/Type/Type.hpp
        Grammar/Type/SimpleType.hpp
        Grammar/Type/ArrayType.hpp

        SymbolTable/Symbol.hpp
        SymbolTable/ScopeLayer.hpp
        SymbolTable/ScopeLayerTree.hpp
        SymbolTable/ClassStorage.hpp

        MemberType/MemberType.hpp
        MemberType/PrimitiveType.hpp
        MemberType/PrimitiveSimpleType.hpp
        MemberType/PrimitiveArrayType.hpp
        MemberType/ClassType.hpp
        MemberType/MethodType.hpp

        Objects/Object.hpp
        Objects/SimpleObject.cpp
        Objects/ArrayObject.cpp

        MethodMechanisms/FunctionTable.hpp
        MethodMechanisms/Frame.hpp

        Grammar/parser.cpp
        main.cpp
        Grammar/driver.cpp
        Grammar/scanner.cpp
)



