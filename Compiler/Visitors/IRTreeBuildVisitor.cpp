#include <IRTree/TreeWrapper/ExpressionWrapper.hpp>
#include <IRTree/TreeWrapper/StatementWrapper.hpp>
#include <IRTree/Nodes/Statement/Statement.hpp>
#include <IRTree/Nodes/Expression/BinopExpression.hpp>
#include <IRTree/Nodes/Expression/MemExpression.hpp>
#include <IRTree/Types/BinaryOperatorType.hpp>
#include <IRTree/Types/LogicOperatorType.hpp>
#include <IRTree/TreeWrapper/ConditionalWrapper/AndConditionalWrapper.hpp>
#include <IRTree/TreeWrapper/ConditionalWrapper/OrConditionalWrapper.hpp>
#include <IRTree/TreeWrapper/ConditionalWrapper/NegateConditionalWrapper.hpp>
#include <IRTree/TreeWrapper/ConditionalWrapper/RelativeConditionalWrapper.hpp>
#include <IRTree/Nodes/ExpressionList.hpp>
#include <IRTree/Nodes/Expression/NameExpression.hpp>
#include <IRTree/Nodes/Expression/CallExpression.hpp>
#include <IRTree/Nodes/Statement/JumpStatement.hpp>
#include <IRTree/Nodes/Statement/LabelStatement.hpp>

#include "TypeVisitor.hpp"
#include "IRTreeBuildVisitor.h"

namespace ast {

IRTreeBuildVisitor::IRTreeBuildVisitor(ScopeLayerTree* scope_layer_tree)
    : scope_layer_tree_(scope_layer_tree),
      type_visitor_(new TypeVisitor(this)) {
  tos_value_ = nullptr;
}

void IRTreeBuildVisitor::Visit(Program* program) {
  program->GetMainClass()->Accept(this);
  if (program->GetClassDeclarationList() != nullptr) {
    program->GetClassDeclarationList()->Accept(this);
  }
}

void IRTreeBuildVisitor::Visit(MainClass* main_class) {
  current_class_name_ = main_class->GetName();
  auto main_method_declaration = new MethodDeclaration(
      new SimpleType("void"), "main", nullptr, main_class->GetStatementList());
  
  main_method_declaration->Accept(this);
}

void IRTreeBuildVisitor::Visit(Expression* expression) {}

void IRTreeBuildVisitor::Visit(AddExpression* add_expression) {
  auto lhs = Accept(add_expression->GetLeftExpression());
  auto rhs = Accept(add_expression->GetRightExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::BinopExpression(
          irt::BinaryOperatorType::PLUS,
          lhs->ToExpression(),
          rhs->ToExpression()
      )
  );
}

void IRTreeBuildVisitor::Visit(AndExpression* and_expression) {
  auto lhs = Accept(and_expression->GetLeftExpression());
  auto rhs = Accept(and_expression->GetRightExpression());
  
  tos_value_ = new irt::AndConditionalWrapper(lhs, rhs);
}

void IRTreeBuildVisitor::Visit(BoolExpression* bool_expression) {
  tos_value_ = new irt::ExpressionWrapper(
      new irt::ConstExpression(bool_expression->GetValue()));
}

void IRTreeBuildVisitor::Visit(DivExpression* div_expression) {
  auto lhs = Accept(div_expression->GetLeftExpression());
  auto rhs = Accept(div_expression->GetRightExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::BinopExpression(
          irt::BinaryOperatorType::DIV,
          lhs->ToExpression(),
          rhs->ToExpression()
      )
  );
}

void IRTreeBuildVisitor::Visit(GreaterEqualExpression* greater_equal_expression) {
  auto lhs = Accept(greater_equal_expression->GetLeftExpression());
  auto rhs = Accept(greater_equal_expression->GetRightExpression());
  
  tos_value_ = new irt::RelativeConditionalWrapper(
      irt::LogicOperatorType::GE,
      lhs->ToExpression(),
      rhs->ToExpression()
  );
}

void IRTreeBuildVisitor::Visit(GreaterExpression* greater_expression) {
  auto lhs = Accept(greater_expression->GetLeftExpression());
  auto rhs = Accept(greater_expression->GetRightExpression());
  
  tos_value_ = new irt::RelativeConditionalWrapper(
      irt::LogicOperatorType::GT,
      lhs->ToExpression(),
      rhs->ToExpression()
  );
}

void IRTreeBuildVisitor::Visit(IdentExpression* ident_expression) {
  auto address = current_frame_->GetAddress(ident_expression->GetIdentifier())->ToExpression();
  tos_value_ = new irt::ExpressionWrapper(address);
}

void IRTreeBuildVisitor::Visit(IntExpression* int_expression) {
  tos_value_ = new irt::ExpressionWrapper(
      new irt::ConstExpression(int_expression->GetValue()));
}

void IRTreeBuildVisitor::Visit(LessEqualExpression* less_equal_expression) {
  auto lhs = Accept(less_equal_expression->GetLeftExpression());
  auto rhs = Accept(less_equal_expression->GetRightExpression());
  
  tos_value_ = new irt::RelativeConditionalWrapper(
      irt::LogicOperatorType::LE,
      lhs->ToExpression(),
      rhs->ToExpression()
  );
}

void IRTreeBuildVisitor::Visit(LessExpression* less_expression) {
  auto lhs = Accept(less_expression->GetLeftExpression());
  auto rhs = Accept(less_expression->GetRightExpression());
  
  tos_value_ = new irt::RelativeConditionalWrapper(
      irt::LogicOperatorType::LT,
      lhs->ToExpression(),
      rhs->ToExpression()
  );
}

void IRTreeBuildVisitor::Visit(MulExpression* mul_expression) {
  auto lhs = Accept(mul_expression->GetLeftExpression());
  auto rhs = Accept(mul_expression->GetRightExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::BinopExpression(
          irt::BinaryOperatorType::MUL,
          lhs->ToExpression(),
          rhs->ToExpression()
      )
  );
}

void IRTreeBuildVisitor::Visit(NotExpression* not_expression) {
  auto expression_wrapper = Accept(not_expression->GetExpression());
  
  tos_value_ = new irt::NegateConditionalWrapper(expression_wrapper);
}

void IRTreeBuildVisitor::Visit(OrExpression* or_expression) {
  auto lhs = Accept(or_expression->GetLeftExpression());
  auto rhs = Accept(or_expression->GetRightExpression());
  
  tos_value_ = new irt::OrConditionalWrapper(lhs, rhs);
}

void IRTreeBuildVisitor::Visit(ModExpression* mod_expression) {
  auto lhs = Accept(mod_expression->GetLeftExpression());
  auto rhs = Accept(mod_expression->GetRightExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::BinopExpression(
          irt::BinaryOperatorType::MOD,
          lhs->ToExpression(),
          rhs->ToExpression()
      )
  );
}

void IRTreeBuildVisitor::Visit(ArrayAccessExpression* array_access_expression) {
  auto array_address = current_frame_->GetAddress(array_access_expression->GetArrayName())->ToExpression();
  auto index = Accept(array_access_expression->GetExpression())->ToExpression();
  
  // TODO: use global const for word size
  tos_value_ = new irt::ExpressionWrapper(
      new irt::MemExpression(
          new irt::BinopExpression(
              irt::BinaryOperatorType::PLUS,
              array_address,
              new irt::BinopExpression(
                  irt::BinaryOperatorType::MUL,
                  new irt::BinopExpression(
                      irt::BinaryOperatorType::PLUS,
                      index,
                      new irt::ConstExpression(1)),
                  new irt::ConstExpression(4)
              )
          )
      )
  );
}

void IRTreeBuildVisitor::Visit(NewArrayExpression* new_array_expression) {
  auto array_size_expression = Accept(new_array_expression->GetExpression())->ToExpression();
  // TODO: use global const for word size
  // TODO: allow to store class objects in array (use GetSize for type)
  auto args = new irt::ExpressionList();
  args->Add(
      new irt::BinopExpression(
          irt::BinaryOperatorType::MUL,
          new irt::ConstExpression(4),
          new irt::BinopExpression(
              irt::BinaryOperatorType::PLUS,
              array_size_expression,
              new irt::ConstExpression(1)
          )
      )
  ); // one additional space for array size
  
  irt::Temporary tmp;
  auto array = new irt::EseqExpression(
      new irt::SeqStatement(
          new irt::MoveStatement(
              new irt::TempExpression(tmp),
              new irt::CallExpression(
                  new irt::NameExpression(irt::Label("malloc")),
                  args
              )
          ),
          new irt::MoveStatement(
              new irt::MemExpression(new irt::TempExpression(tmp)),
              array_size_expression
          )
      ),
      new irt::TempExpression(tmp)
  );
  
  tos_value_ = new irt::ExpressionWrapper(array);
}

void IRTreeBuildVisitor::Visit(SubExpression* sub_expression) {
  auto lhs = Accept(sub_expression->GetLeftExpression());
  auto rhs = Accept(sub_expression->GetRightExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::BinopExpression(
          irt::BinaryOperatorType::MINUS,
          lhs->ToExpression(),
          rhs->ToExpression()
      )
  );
}

void IRTreeBuildVisitor::Visit(EqualExpression* equal_expression) {
  auto lhs = Accept(equal_expression->GetLeftExpression());
  auto rhs = Accept(equal_expression->GetRightExpression());
  
  tos_value_ = new irt::RelativeConditionalWrapper(
      irt::LogicOperatorType::EQ,
      lhs->ToExpression(),
      rhs->ToExpression()
  );
}

void IRTreeBuildVisitor::Visit(ThisExpression* this_expression) {
  auto address_expression = current_frame_->GetAddress("this")->ToExpression();
  
  tos_value_ = new irt::ExpressionWrapper(address_expression);
}

void IRTreeBuildVisitor::Visit(MethodInvocationExpression* method_invocation_expression) {
  method_invocation_expression->GetMethodInvocation()->Accept(this);
}

void IRTreeBuildVisitor::Visit(ExpressionList* expression_list) {}

void IRTreeBuildVisitor::Visit(NewExpression* new_expression) {
  auto type = new PrimitiveSimpleType(new_expression->GetIdentifier());
  
  auto args = new irt::ExpressionList();
  args->Add(new irt::ConstExpression(type->GetSize()));
  
  irt::Temporary tmp;
  auto object = new irt::EseqExpression(
      new irt::MoveStatement(
          new irt::TempExpression(tmp),
          new irt::CallExpression(
              new irt::NameExpression(irt::Label("malloc")),
              args
          )
      ),
      new irt::TempExpression(tmp)
  );
  
  tos_value_ = new irt::ExpressionWrapper(object);
}

void IRTreeBuildVisitor::Visit(ArrayLengthExpression* array_length_expression) {
  // TODO: add support for complex expressions
  // Type resolver is required here
  std::string array_name = dynamic_cast<IdentExpression*>(
      array_length_expression->GetExpression())->GetIdentifier();
  auto array_address = current_frame_->GetAddress(array_name)->ToExpression();
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::MemExpression(
          array_address
      )
  );
}

// Base class
void IRTreeBuildVisitor::Visit(Declaration* declaration) {}

void IRTreeBuildVisitor::Visit(DeclarationList* declaration_list) {
  if (declaration_list->GetFirstItem() == nullptr) {
    return;
  }
  declaration_list->GetFirstItem()->Accept(this);
  if (declaration_list->GetTail() != nullptr) {
    declaration_list->GetTail()->Accept(this);
  }
}

void IRTreeBuildVisitor::Visit(VariableDeclaration* variable_declaration) {
  tos_value_ = nullptr;
  current_frame_->AddLocalVariable(variable_declaration->GetVariableName());
}

void IRTreeBuildVisitor::Visit(ClassDeclaration* class_declaration) {
  current_class_name_ = class_declaration->GetClassName();
  if (class_declaration->GetDeclarationList() != nullptr) {
    class_declaration->GetDeclarationList()->Accept(this);
  }
}

void IRTreeBuildVisitor::Visit(MethodDeclaration* method_declaration) {
  layer_indices_.push(0);
  
  current_layer_ = scope_layer_tree_->GetScopeLayer(method_declaration->GetIdentifier());
  
  std::string full_method_name;
  if (method_declaration->GetIdentifier() != "main") {
    full_method_name = current_class_name_ + "_" + method_declaration->GetIdentifier();
  } else {
    full_method_name = "main";
  }
  
  current_frame_ = new irt::FrameTranslator(full_method_name);
  frame_translator_[full_method_name] = current_frame_;
  
  current_frame_->AddArgumentAddress("this");
  if (method_declaration->GetFormalList() != nullptr) {
    method_declaration->GetFormalList()->Accept(this);
  }
  // current_frame_->AddReturnAddress(); - stored in link register
  
  auto statement_list = Accept(method_declaration->GetStatementList());
  
  if (statement_list) {
    tos_value_ = new irt::StatementWrapper(
        new irt::SeqStatement(
            new irt::LabelStatement(irt::Label(full_method_name)),
            statement_list->ToStatement()
        )
    );
  } else {
    tos_value_ = nullptr;
  }
  
  layer_indices_.pop();
  method_trees_.emplace(
      full_method_name,
      std::make_pair(
          current_frame_,
          tos_value_->ToStatement()
      )
  );
}

void IRTreeBuildVisitor::Visit(ClassDeclarationList* class_declaration_list) {
  if (class_declaration_list->GetFirstItem() == nullptr) {
    return;
  }
  class_declaration_list->GetFirstItem()->Accept(this);
  if (class_declaration_list->GetTail() != nullptr) {
    class_declaration_list->GetTail()->Accept(this);
  }
}

void IRTreeBuildVisitor::Visit(Formal* formal) {
  current_frame_->AddArgumentAddress(formal->GetIdentifier());
}

void IRTreeBuildVisitor::Visit(FormalList* formal_list) {
  while (formal_list != nullptr) {
    Formal* formal = formal_list->GetFirstItem();
    if (formal == nullptr) {
      return;
    }
    
    formal->Accept(this);
    formal_list = formal_list->GetTail();
  }
}

void IRTreeBuildVisitor::Visit(MethodInvocation* method_invocation) {
  auto args = new irt::ExpressionList();
  // first argument is object itself
  args->Add(Accept(method_invocation->GetExpression())->ToExpression());
  
  ExpressionList* expression_list = method_invocation->GetExpressionList();
  while (expression_list != nullptr) {
    Expression* expression = expression_list->GetFirstItem();
    if (expression == nullptr) {
      break;
    }
    args->Add(Accept(expression)->ToExpression());
    expression_list = expression_list->GetTail();
  }
  
  std::string class_name = type_visitor_->Accept(method_invocation->GetExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::CallExpression(
          new irt::NameExpression(
              irt::Label(class_name + "_" + method_invocation->GetIdentifier())),
          args
      )
  );
}

void IRTreeBuildVisitor::Visit(Statement* statement) {}

void IRTreeBuildVisitor::Visit(StatementList* statement_list) {
  if (statement_list->GetHead() == nullptr) {
    tos_value_ = nullptr;
    return;
  }
  
  std::stack<irt::Statement*> statements;
  while (statement_list != nullptr) {
    Statement* statement = statement_list->GetHead();
    if (statement == nullptr) {
      return;
    }
    auto statement_wrapper = Accept(statement);
    if (statement_wrapper) {
      statements.push(statement_wrapper->ToStatement());
    }
    statement_list = statement_list->GetTail();
  }
  
  irt::Statement* suffix = statements.top();
  statements.pop();
  
  while (!statements.empty()) {
    irt::Statement* statement = statements.top();
    statements.pop();
    suffix = new irt::SeqStatement(
        statement,
        suffix
    );
  }
  tos_value_ = new irt::StatementWrapper(suffix);
}

void IRTreeBuildVisitor::Visit(AssignmentStatement* assignment_statement) {
  auto named_entity_expression = Accept(assignment_statement->GetNamedEntity())->ToExpression();
  auto expression = Accept(assignment_statement->GetExpression())->ToExpression();
  
  tos_value_ = new irt::StatementWrapper(
      new irt::MoveStatement(named_entity_expression, expression));
}

void IRTreeBuildVisitor::Visit(IfStatement* if_statement) {
  auto condition_expression = Accept(if_statement->GetExpression());
  auto true_statement = Accept(if_statement->GetStatement());
  
  irt::Label label_true;
  irt::Label label_join;
  
  irt::Statement* suffix = new irt::LabelStatement(label_join);
  
  irt::Label* result_true = &label_join;
  irt::Label* result_false = &label_join;
  
  // Sequentially append instructions to one another
  if (true_statement) {
    result_true = &label_true;
    suffix = new irt::SeqStatement(
        new irt::LabelStatement(label_true),
        new irt::SeqStatement(
            true_statement->ToStatement(),
            suffix
        )
    );
  }
  
  tos_value_ = new irt::StatementWrapper(
      new irt::SeqStatement(
          condition_expression->ToConditional(*result_true, *result_false),
          suffix
      )
  );
}

void IRTreeBuildVisitor::Visit(PrintStatement* print_statement) {
  auto args = new irt::ExpressionList();
  args->Add(Accept(print_statement->GetExpression())->ToExpression());
  
  tos_value_ = new irt::ExpressionWrapper(
      new irt::CallExpression(
          new irt::NameExpression(irt::Label("printf")),
          args
      )
  );
}

void IRTreeBuildVisitor::Visit(ReturnStatement* return_statement) {
  auto return_expression = Accept(return_statement->GetExpression())->ToExpression();
  tos_value_ = new irt::StatementWrapper(
      new irt::MoveStatement(
          current_frame_->GetReturnValueAddress()->ToExpression(),
          return_expression
      )
  );
}

void IRTreeBuildVisitor::Visit(VariableDeclarationStatement* variable_declaration_statement) {
  variable_declaration_statement->GetVariableDeclaration()->Accept(this);
}

void IRTreeBuildVisitor::Visit(WhileStatement* while_statement) {
  auto condition_expression = Accept(while_statement->GetExpression());
  auto while_statement_wrapper = Accept(while_statement->GetStatement());
  
  irt::Label label_condition;
  irt::Label label_true;
  irt::Label label_join;
  
  irt::Statement* suffix = new irt::LabelStatement(label_join);
  
  irt::Label* result_true = &label_join;
  
  if (while_statement_wrapper) {
    result_true = &label_true;
    suffix = new irt::SeqStatement(
        new irt::LabelStatement(label_true),
        new irt::SeqStatement(
            while_statement_wrapper->ToStatement(),
            new irt::SeqStatement(
                new irt::JumpStatement(label_condition),
                suffix
            )
        )
    );
  }
  
  suffix = new irt::SeqStatement(
      new irt::LabelStatement(label_condition),
      new irt::SeqStatement(
          condition_expression->ToConditional(*result_true, label_join),
          suffix
      )
  );
  
  tos_value_ = new irt::StatementWrapper(suffix);
}

void IRTreeBuildVisitor::Visit(AssertStatement* assert_statement) {
  auto args = new irt::ExpressionList();
  args->Add(Accept(assert_statement->GetExpression())->ToExpression());
  
  // TODO: add label 'assert' and use function from standard library (exit)
  tos_value_ = new irt::ExpressionWrapper(
      new irt::CallExpression(
          new irt::NameExpression(irt::Label("assert")),
          args
      )
  );
}

void IRTreeBuildVisitor::Visit(IfElseStatement* if_else_statement) {
  auto condition_expression = Accept(if_else_statement->GetExpression());
  auto true_statement = Accept(if_else_statement->GetFirstStatement());
  auto false_statement = Accept(if_else_statement->GetSecondStatement());
  
  irt::Label label_true;
  irt::Label label_false;
  irt::Label label_join;
  
  irt::Statement* suffix = new irt::LabelStatement(label_join);
  
  irt::Label* result_true = &label_join;
  irt::Label* result_false = &label_join;
  
  // Sequentially append instructions to one another
  if (false_statement) {
    result_false = &label_false;
    suffix = new irt::SeqStatement(
        new irt::LabelStatement(label_false),
        new irt::SeqStatement(
            false_statement->ToStatement(),
            suffix
        )
    );
    
    if (true_statement) {
      suffix = new irt::SeqStatement(
          new irt::JumpStatement(label_join),
          suffix
      );
    }
  }
  
  if (true_statement) {
    result_true = &label_true;
    suffix = new irt::SeqStatement(
        new irt::LabelStatement(label_true),
        new irt::SeqStatement(
            true_statement->ToStatement(),
            suffix
        )
    );
  }
  
  tos_value_ = new irt::StatementWrapper(
      new irt::SeqStatement(
          condition_expression->ToConditional(*result_true, *result_false),
          suffix
      )
  );
}

void IRTreeBuildVisitor::Visit(ScopeStatement* scope_statement) {
  current_layer_ = current_layer_->GetChild(layer_indices_.top());
  
  layer_indices_.push(0);
  current_frame_->SetupScope();
  
  scope_statement->GetStatementList()->Accept(this);
  
  layer_indices_.pop();
  size_t index = layer_indices_.top();
  
  layer_indices_.pop();
  layer_indices_.push(index + 1);
  
  current_layer_ = current_layer_->GetParent();
  current_frame_->TearDownScope();
}

void IRTreeBuildVisitor::Visit(MethodInvocationStatement* method_invocation_statement) {
  method_invocation_statement->GetMethodInvocation()->Accept(this);
}

// Base class
void IRTreeBuildVisitor::Visit(NamedEntity* named_entity) {}

void IRTreeBuildVisitor::Visit(NamedVariable* named_variable) {
  auto variable = current_frame_->GetAddress(named_variable->GetName())->ToExpression();
  tos_value_ = new irt::ExpressionWrapper(variable);
}

void IRTreeBuildVisitor::Visit(NamedArrayElement* named_array_element) {
  auto array_address = current_frame_->GetAddress(named_array_element->GetName())->ToExpression();
  auto index = Accept(named_array_element->GetIndexExpression())->ToExpression();
  
  auto element_address = new irt::BinopExpression(
      irt::BinaryOperatorType::PLUS,
      array_address,
      new irt::BinopExpression(
          irt::BinaryOperatorType::MUL,
          new irt::BinopExpression(
              irt::BinaryOperatorType::PLUS,
              index,
              new irt::ConstExpression(1)
          ),
          new irt::ConstExpression(4)
      )
  );
  
  tos_value_ = new irt::ExpressionWrapper(new irt::MemExpression(element_address));
}

// Base class
void IRTreeBuildVisitor::Visit(Type* type) {}

void IRTreeBuildVisitor::Visit(SimpleType* simple_type) {}

void IRTreeBuildVisitor::Visit(ArrayType* array_type) {}

ScopeLayer* IRTreeBuildVisitor::GetCurrentLayer() const {
  return current_layer_;
}

std::string IRTreeBuildVisitor::GetCurrentClassName() const {
  return current_class_name_;
}

IrtMapping IRTreeBuildVisitor::GetMethodTrees() {
  return method_trees_;
}

} // namespace ast