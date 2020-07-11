#include "MethodCallVisitor.h"

namespace ast {

MethodCallVisitor::MethodCallVisitor(ScopeLayer* method_scope,
                                     std::shared_ptr<MethodType> method,
                                     SimpleObject* this_object)
    : frame_((method->GetFormalList() != nullptr) ? method->GetFormalList()->GetSize() : 0),
      root_layer_(method_scope),
      this_object_(this_object),
      current_layer_(method_scope) {
  layer_indices_.push(0);

  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), 0);

  // TODO: add fields support in frame
}

void MethodCallVisitor::SetTree(ScopeLayerTree* tree) {
  scope_layer_tree_ = tree;
}

void MethodCallVisitor::SetParams(const std::vector<Object*>& params) {
  frame_.SetParams(params);
}

Frame& MethodCallVisitor::GetFrame() {
  return frame_;
}

void MethodCallVisitor::Visit(Program* program) {}

void MethodCallVisitor::Visit(MainClass* main_class) {}

// Base class
void MethodCallVisitor::Visit(Expression* expression) {}

std::pair<int, int> MethodCallVisitor::VisitBinaryExpression(Expression* left_expression,
                                                             Expression* right_expression) {
  Object* left_object = Accept(left_expression);
  Object* right_object = Accept(right_expression);

  PrimitiveType* left_type = left_object->GetType();
  PrimitiveType* right_type = right_object->GetType();
//  if (*left_type != *right_type) {
//    throw std::runtime_error("Types mismatch for binary operator");
//  }

  if ((left_type->GetTypeName() != "int" && left_type->GetTypeName() != "boolean") ||
    (right_type->GetTypeName() != "int" && right_type->GetTypeName() != "boolean")) {
    throw std::runtime_error("Bad type for arithmetic/logic operations");
  }

  int left_value = left_object->GetValue();
  int right_value = right_object->GetValue();
  return std::make_pair(left_value, right_value);
}

void MethodCallVisitor::Visit(AddExpression* add_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(add_expression->GetLeftExpression(),
                                                        add_expression->GetRightExpression());
  int result = left_value + right_value;
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), result);
}

void MethodCallVisitor::Visit(AndExpression* and_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(and_expression->GetLeftExpression(),
                                                        and_expression->GetRightExpression());
  int result = static_cast<bool>(left_value) && static_cast<bool>(right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(BoolExpression* bool_expression) {
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), bool_expression->GetValue());
}

void MethodCallVisitor::Visit(DivExpression* div_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(div_expression->GetLeftExpression(),
                                                        div_expression->GetRightExpression());
  int result = left_value / right_value;
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), result);
}

void MethodCallVisitor::Visit(GreaterEqualExpression* greater_equal_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(greater_equal_expression->GetLeftExpression(),
                                                        greater_equal_expression->GetRightExpression());
  int result = static_cast<int>(left_value >= right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(GreaterExpression* greater_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(greater_expression->GetLeftExpression(),
                                                        greater_expression->GetRightExpression());
  int result = static_cast<int>(left_value > right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(IdentExpression* ident_expression) {
  int index = function_table_.Get(Symbol(ident_expression->GetIdentifier()));
  tos_value_ = frame_.Get(index);
}

void MethodCallVisitor::Visit(IntExpression* int_expression) {
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), int_expression->GetValue());
}

void MethodCallVisitor::Visit(LessEqualExpression* less_equal_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(less_equal_expression->GetLeftExpression(),
                                                        less_equal_expression->GetRightExpression());
  int result = static_cast<int>(left_value <= right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(LessExpression* less_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(less_expression->GetLeftExpression(),
                                                        less_expression->GetRightExpression());
  int result = static_cast<int>(left_value < right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(MulExpression* mul_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(mul_expression->GetLeftExpression(),
                                                        mul_expression->GetRightExpression());
  int result = left_value * right_value;
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), result);
}

void MethodCallVisitor::Visit(NotExpression* not_expression) {
  Object* object = Accept(not_expression->GetExpression());
  if (object->GetType()->GetTypeName() != "boolean" && object->GetType()->GetTypeName() != "int") {
    throw std::runtime_error("Bad type for not operator: " + object->GetType()->GetTypeName());
  }
  int result = !static_cast<bool>(object->GetValue());
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(OrExpression* or_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(or_expression->GetLeftExpression(),
                                                        or_expression->GetRightExpression());
  int result = static_cast<bool>(left_value) || static_cast<bool>(right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(ModExpression* mod_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(mod_expression->GetLeftExpression(),
                                                        mod_expression->GetRightExpression());
  int result = left_value % right_value;
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), result);
}

void MethodCallVisitor::Visit(ArrayAccessExpression* array_access_expression) {
  int array_index = Accept(array_access_expression->GetExpression())->GetValue();
  int stack_index = function_table_.Get(Symbol(array_access_expression->GetArrayName()));
  tos_value_ = dynamic_cast<ArrayObject*>(frame_.Get(stack_index))->GetAtIndex(array_index);
}

void MethodCallVisitor::Visit(NewArrayExpression* new_array_expression) {
  auto new_array = new ArrayObject(new PrimitiveArrayType(
      new PrimitiveSimpleType(new_array_expression->GetSimpleType())));
  size_t new_array_size = Accept(new_array_expression->GetExpression())->GetValue();
  new_array->Resize(new_array_size);
  tos_value_ = new_array;
}

void MethodCallVisitor::Visit(SubExpression* sub_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(sub_expression->GetLeftExpression(),
                                                        sub_expression->GetRightExpression());
  int result = left_value - right_value;
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), result);
}

void MethodCallVisitor::Visit(EqualExpression* equal_expression) {
  auto[left_value, right_value] = VisitBinaryExpression(equal_expression->GetLeftExpression(),
                                                        equal_expression->GetRightExpression());
  int result = (left_value == right_value);
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("boolean"), result);
}

void MethodCallVisitor::Visit(ThisExpression* this_expression) {
  tos_value_ = this_object_;
}

void MethodCallVisitor::Visit(MethodInvocationExpression* method_invocation_expression) {
  tos_value_ = Accept(method_invocation_expression->GetMethodInvocation());
}

// Function params are counted in Visit for method_invocation
void MethodCallVisitor::Visit(ExpressionList* expression_list) {}

void MethodCallVisitor::Visit(NewExpression* new_expression) {
  tos_value_ = new SimpleObject(new PrimitiveSimpleType(new_expression->GetIdentifier()));
}

void MethodCallVisitor::Visit(ArrayLengthExpression* array_length_expression) {
  auto array_object = dynamic_cast<ArrayObject*>(Accept(array_length_expression->GetExpression()));
  tos_value_ = new SimpleObject(new PrimitiveSimpleType("int"), array_object->GetSize());
}

// Base class
void MethodCallVisitor::Visit(Declaration* declaration) {}

void MethodCallVisitor::Visit(DeclarationList* declaration_list) {
  if (declaration_list->GetFirstItem() == nullptr) {
    return;
  }
  declaration_list->GetFirstItem()->Accept(this);
  if (declaration_list->GetTail() == nullptr) {
    return;
  }
  declaration_list->GetTail()->Accept(this);
}

void MethodCallVisitor::Visit(VariableDeclaration* variable_declaration) {
  int index;
  if (variable_declaration->GetType()->IsSimpleType()) {
    index = frame_.AllocVariable(new PrimitiveSimpleType(
        new SimpleType(variable_declaration->GetType()->GetIdentifier())));
  } else {
    index = frame_.AllocVariable(new PrimitiveArrayType(
        new ArrayType(variable_declaration->GetType()->GetIdentifier())));
  }
  function_table_.CreateVariable(variable_declaration->GetVariableName());
  function_table_.Put(variable_declaration->GetVariableName(), index);
}

void MethodCallVisitor::Visit(ClassDeclaration* class_declaration) {}

void MethodCallVisitor::Visit(MethodDeclaration* method_declaration) {
  if (method_declaration->GetFormalList() != nullptr) {
    method_declaration->GetFormalList()->Accept(this);
  }
  method_declaration->GetStatementList()->Accept(this);
}

void MethodCallVisitor::Visit(ClassDeclarationList* class_declaration_list) {}

void MethodCallVisitor::Visit(Formal* formal) {}

void MethodCallVisitor::Visit(FormalList* formal_list) {
  // TODO: solve case with class fields
  int index = -1;
  while (formal_list != nullptr) {
    Formal* formal = formal_list->GetFirstItem();
    if (formal == nullptr) {
      return;
    }

    function_table_.CreateVariable(formal->GetIdentifier());
    function_table_.Put(formal->GetIdentifier(), index);
    --index;

    formal_list = formal_list->GetTail();
  }
}

void MethodCallVisitor::Visit(MethodInvocation* method_invocation) {
  std::cout << "Method has been called: " << method_invocation->GetIdentifier() << std::endl;

  auto object = dynamic_cast<SimpleObject*>(Accept(method_invocation->GetExpression()));

  Symbol class_name = object->GetType()->GetTypeName();
  ClassStorage::GetInstance().Output();

  auto& class_methods = ClassStorage::GetInstance().GetClassMethods(class_name);

  std::vector<Object*> params;
  ExpressionList* expression_list = method_invocation->GetExpressionList();
  while (expression_list != nullptr) {
    Expression* expression = expression_list->GetFirstItem();
    if (expression == nullptr) {
      break;
    }
    params.push_back(Accept(expression));
    expression_list = expression_list->GetTail();
  }

  shared_ptr<MethodType> method = class_methods[method_invocation->GetIdentifier()];

  MethodCallVisitor visitor(
      scope_layer_tree_->GetScopeLayer(method_invocation->GetIdentifier()),
      method, object);

  visitor.SetParams(params);
  visitor.GetFrame().SetParentFrame(&frame_);
  visitor.SetTree(scope_layer_tree_);
  visitor.Visit(method->GetMethodDeclaration());
  tos_value_ = visitor.GetFrame().GetReturnValue();
}

// Base class
void MethodCallVisitor::Visit(Statement* statement) {}

void MethodCallVisitor::Visit(StatementList* statement_list) {
  if (statement_list->GetHead() == nullptr) {
    return;
  }
  statement_list->GetHead()->Accept(this);
  if (statement_list->GetTail() != nullptr) {
    statement_list->GetTail()->Accept(this);
  }
}

void MethodCallVisitor::Visit(AssignmentStatement* assignment_statement) {
  Object* value = Accept(assignment_statement->GetExpression());

  int stack_index = function_table_.Get(assignment_statement->GetNamedEntity()->GetName());
  if (assignment_statement->GetNamedEntity()->IsVariable()) {
    frame_.Set(stack_index, value);
  } else {
    int array_index = Accept(assignment_statement->GetNamedEntity())->GetValue();
    dynamic_cast<ArrayObject*>(frame_.Get(stack_index))->SetAtIndex(array_index, value);
  }
}

void MethodCallVisitor::Visit(IfStatement* if_statement) {
  if (Accept(if_statement->GetExpression())->GetValue()) {
    if_statement->GetStatement()->Accept(this);
  } else {
    // Skip 'true' scope
    int index = layer_indices_.top();
    layer_indices_.pop();
    layer_indices_.push(index + 1);
  }
}

void MethodCallVisitor::Visit(PrintStatement* print_statement) {
  std::cout << Accept(print_statement->GetExpression())->GetValue() << std::endl;
}

void MethodCallVisitor::Visit(ReturnStatement* return_statement) {
  frame_.SetReturnValue(Accept(return_statement->GetExpression()));
  is_returned_ = true;
}

void MethodCallVisitor::Visit(VariableDeclarationStatement* variable_declaration_statement) {
  variable_declaration_statement->GetVariableDeclaration()->Accept(this);
}

void MethodCallVisitor::Visit(WhileStatement* while_statement) {
  int expression_value = Accept(while_statement->GetExpression())->GetValue();
  while (expression_value) {
    while_statement->GetStatement()->Accept(this);
    expression_value = Accept(while_statement->GetExpression())->GetValue();
    if (expression_value) {
      // The next scope is the same
      int index = layer_indices_.top();
      layer_indices_.pop();
      layer_indices_.push(index - 1);
    }
  }
}

void MethodCallVisitor::Visit(AssertStatement* assert_statement) {
  if (!Accept(assert_statement->GetExpression())->GetValue()) {
    throw std::runtime_error("Assert failed");
  }
}

void MethodCallVisitor::Visit(IfElseStatement* if_else_statement) {
  if (Accept(if_else_statement->GetExpression())->GetValue()) {
    if_else_statement->GetFirstStatement()->Accept(this);
    // Skip 'false' scope
    int index = layer_indices_.top();
    layer_indices_.pop();
    layer_indices_.push(index + 1);
  } else {
    // Skip 'true' scope
    int index = layer_indices_.top();
    layer_indices_.pop();
    layer_indices_.push(index + 1);
    if_else_statement->GetSecondStatement()->Accept(this);
  }
}

void MethodCallVisitor::Visit(ScopeStatement* scope_statement) {
  std::cout << "----------------------------------------->" << std::endl;
  std::cout << "Entering new scope..." << std::endl;
  // scope_layer_tree_->OutputTree();

  current_layer_ = current_layer_->GetChild(layer_indices_.top());

  layer_indices_.push(0);
  frame_.AllocScope(); // save old size of stack
  function_table_.BeginScope(); // mark start of scope

  scope_statement->GetStatementList()->Accept(this);

  layer_indices_.pop();
  size_t index = layer_indices_.top();

  layer_indices_.pop();
  layer_indices_.push(index + 1);

  current_layer_ = current_layer_->GetParent();
  frame_.DeallocScope(); // restore old stack size
  function_table_.EndScope(); // remove scope symbols from stack

  std::cout << "Exiting scope..." << std::endl;
  std::cout << "<-----------------------------------------" << std::endl;
}

void MethodCallVisitor::Visit(MethodInvocationStatement* method_invocation_statement) {
  method_invocation_statement->GetMethodInvocation()->Accept(this);
}

// Base class
void MethodCallVisitor::Visit(NamedEntity* named_entity) {}

void MethodCallVisitor::Visit(NamedVariable* named_variable) {}

void MethodCallVisitor::Visit(NamedArrayElement* named_array_element) {
  tos_value_ = Accept(named_array_element->GetIndexExpression());
}

void MethodCallVisitor::Visit(Type* type) {}

void MethodCallVisitor::Visit(SimpleType* simple_type) {}

void MethodCallVisitor::Visit(ArrayType* array_type) {}

} // namespace ast
