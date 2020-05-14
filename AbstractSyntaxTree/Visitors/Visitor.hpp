#pragma once

#include "forward_decl.hpp"

namespace ast {

class Visitor {
 public:
    virtual void Visit(Program* program) = 0;
    virtual void Visit(MainClass* main_class) = 0;
    virtual void Visit(Expression* expression) = 0;
    virtual void Visit(AddExpression* add_expression) = 0;
    virtual void Visit(AndExpression* and_expression) = 0;
    virtual void Visit(BoolExpression* bool_expression) = 0;
    virtual void Visit(DivExpression* div_expression) = 0;
    virtual void Visit(GreaterEqualExpression* greater_equal_expression) = 0;
    virtual void Visit(GreaterExpression* greater_expression) = 0;
    virtual void Visit(IdentExpression* ident_expression) = 0;
    virtual void Visit(IntExpression* int_expression) = 0;
    virtual void Visit(LessEqualExpression* less_equal_expression) = 0;
    virtual void Visit(LessExpression* less_expression) = 0;
    virtual void Visit(MulExpression* mul_expression) = 0;
    virtual void Visit(NotExpression* not_expression) = 0;
    virtual void Visit(OrExpression* or_expression) = 0;
    virtual void Visit(SubExpression* sub_expression) = 0;
    virtual void Visit(EqualExpression* equal_expression) = 0;
    virtual void Visit(ModExpression* mod_expression) = 0;

    virtual void Visit(ArrayAccessExpression* array_access_expression) = 0;

    virtual void Visit(Declaration* declaration) = 0;
    virtual void Visit(DeclarationList* declaration_list) = 0;
    virtual void Visit(VariableDeclaration* variable_declaration) = 0;

    virtual void Visit(ArrayDeclaration* array_declaration) = 0;

    virtual void Visit(Statement* statement) = 0;
    virtual void Visit(StatementList* statement_list) = 0;
    virtual void Visit(AssignmentStatement* assignment_statement) = 0;
    virtual void Visit(IfStatement* if_statement) = 0;
    virtual void Visit(PrintStatement* print_statement) = 0;
    virtual void Visit(ReturnStatement* return_statement) = 0;
    virtual void Visit(VariableDeclarationStatement* variable_declaration_statement) = 0;
    virtual void Visit(WhileStatement* while_statement) = 0;
    virtual void Visit(AssertStatement* assert_statement) = 0;
    virtual void Visit(IfElseStatement* if_else_statement) = 0;

    virtual void Visit(ArrayDeclarationStatement* array_declaration_statement) = 0;
    virtual void Visit(ArrayAssignmentStatement* array_assignment_statement) = 0;

    virtual void Visit(ArrayElementAssignmentStatement* array_element_assignment_statement) = 0;

    virtual void Visit(NamedEntity* named_entity) = 0;
    virtual void Visit(NamedVariable* named_variable) = 0;

    virtual void Visit(NamedArray* named_array) = 0;

    virtual void Visit(Type* type) = 0;
    virtual void Visit(SimpleType* simple_type) = 0;
    virtual void Visit(ArrayType* array_type) = 0;
};

} // namespace ast