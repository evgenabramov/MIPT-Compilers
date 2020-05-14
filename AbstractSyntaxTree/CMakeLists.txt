cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0076 NEW)

set(
    PROJECT_NAME
    AbstractSyntaxTree
)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
    HEADERS
    ${CMAKE_SOURCE_DIR}/driver.hh
    ${CMAKE_SOURCE_DIR}/parser.hh
)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)    

BISON_TARGET(
    MyParser
    parser.y
    ${CMAKE_SOURCE_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_SOURCE_DIR}/parser.hh
    COMPILE_FLAGS --graph
    VERBOSE ${CMAKE_SOURCE_DIR}/found_conflicts.txt
)

FLEX_TARGET(
    MyScanner
    scanner.l
    ${CMAKE_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    driver.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    Service
    Visitors
    Expression
    Declaration
    Statement
    NamedEntity
    Types
)

target_sources(
    ${PROJECT_NAME} 
    PUBLIC
    
    Expression/Expression.hpp
    Expression/AddExpression.hpp
    Expression/AndExpression.hpp
    Expression/BoolExpression.hpp
    Expression/DivExpression.hpp
    Expression/GreaterEqualExpression.hpp
    Expression/GreaterExpression.hpp
    Expression/IdentExpression.hpp
    Expression/IntExpression.hpp
    Expression/LessEqualExpression.hpp
    Expression/LessExpression.hpp
    Expression/MulExpression.hpp
    Expression/NotExpression.hpp
    Expression/OrExpression.hpp
    Expression/SubExpression.hpp
    Expression/ModExpression.hpp
    Expression/EqualExpression.hpp
    
    Declaration/Declaration.hpp
    Declaration/DeclarationList.hpp
    Declaration/VariableDeclaration.hpp

    Statement/Statement.hpp
    Statement/StatementList.hpp
    Statement/AssignmentStatement.hpp
    Statement/IfStatement.hpp
    Statement/PrintStatement.hpp
    Statement/ReturnStatement.hpp
    Statement/VariableDeclarationStatement.hpp
    Statement/WhileStatement.hpp
    Statement/AssertStatement.hpp
    Statement/IfElseStatement.hpp

    Service/TreeNode.hpp
    Service/MainClass.hpp
    Service/Program.hpp
    Service/forward_decl.hpp

    Visitors/Visitor.hpp
    Visitors/include_list.hpp
    Visitors/PrintVisitor.hpp
    Visitors/Interpreter.hpp

    NamedEntity/NamedEntity.hpp
    NamedEntity/NamedVariable.hpp

    Types/Type.hpp
    Types/SimpleType.hpp
    Types/ArrayType.hpp

    Visitors/InterpreterValues/Value.hpp
    Visitors/InterpreterValues/IntValue.hpp
    Visitors/InterpreterValues/BoolValue.hpp

    parser.cpp
    main.cpp
    driver.cpp
    scanner.cpp
)



