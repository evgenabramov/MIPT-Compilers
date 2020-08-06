#include "BlockBorderVisitor.h"

#include "BaseElements.h"

namespace irt {

BlockBorderVisitor::BlockBorderVisitor(std::string method_name) : method_name_(std::move(method_name)) {}

void BlockBorderVisitor::Visit(ExpStatement* statement) {
  IrtStorage child_expression = Accept(statement->GetExpression());
  tos_value_.statement_ = new ExpStatement(child_expression.expression_);
}

void BlockBorderVisitor::Visit(ConstExpression* const_expression) {
  tos_value_.expression_ = new ConstExpression(const_expression->Value());
}

void BlockBorderVisitor::Visit(JumpConditionalStatement* jump_conditional_statement) {
  is_block_finished_ = true;
  
  IrtStorage lhs = Accept(jump_conditional_statement->left_operand_);
  IrtStorage rhs = Accept(jump_conditional_statement->right_operand_);
  
  tos_value_.statement_ = new JumpConditionalStatement(
      jump_conditional_statement->operator_type_,
      lhs.expression_,
      rhs.expression_,
      jump_conditional_statement->label_true_,
      jump_conditional_statement->label_false_
  );
}

void BlockBorderVisitor::Visit(MoveStatement* move_statement) {
  IrtStorage target = Accept(move_statement->target_);
  IrtStorage source = Accept(move_statement->source_);
  
  tos_value_.statement_ = new MoveStatement(
      target.expression_,
      source.expression_
  );
}

void BlockBorderVisitor::Visit(SeqStatement* seq_statement) {
  if (seq_statement->first_statement_->IsLabelStatement()) {
    auto label_statement = dynamic_cast<LabelStatement*>(seq_statement->first_statement_);
    
    if (!is_block_finished_) {
      is_block_finished_ = true;
      
      IrtStorage first_statement = Accept(seq_statement->first_statement_);
      IrtStorage second_statement = Accept(seq_statement->second_statement_);
      
      tos_value_.statement_ = new SeqStatement(
          new JumpStatement(label_statement->label_),
          new SeqStatement(
              first_statement.statement_,
              second_statement.statement_
          )
      );
      return;
    } else {
      is_block_finished_ = false;
    }
  }
  
  IrtStorage first_statement = Accept(seq_statement->first_statement_);
  IrtStorage second_statement = Accept(seq_statement->second_statement_);
  
  tos_value_.statement_ = new SeqStatement(
      first_statement.statement_,
      second_statement.statement_
  );
}

void BlockBorderVisitor::Visit(LabelStatement* label_statement) {
  tos_value_.statement_ = new LabelStatement(label_statement->label_);
}

void BlockBorderVisitor::Visit(BinopExpression* binop_expression) {
  IrtStorage first = Accept(binop_expression->first_);
  IrtStorage second = Accept(binop_expression->second_);
  
  tos_value_.expression_ = new BinopExpression(
      binop_expression->operator_type_,
      first.expression_,
      second.expression_
  );
}

void BlockBorderVisitor::Visit(TempExpression* temp_expression) {
  tos_value_.expression_ = new TempExpression(temp_expression->temporary_);
}

void BlockBorderVisitor::Visit(MemExpression* mem_expression) {
  IrtStorage child = Accept(mem_expression->expression_);
  
  tos_value_.expression_ = new MemExpression(child.expression_);
}

void BlockBorderVisitor::Visit(JumpStatement* jump_statement) {
  is_block_finished_ = true;
  tos_value_.statement_ = new JumpStatement(jump_statement->label_);
}

void BlockBorderVisitor::Visit(CallExpression* call_expression) {
  IrtStorage func = Accept(call_expression->function_name_);
  IrtStorage args = Accept(call_expression->args_);
  
  tos_value_.expression_ = new CallExpression(
      func.expression_,
      args.expression_list_
  );
}

void BlockBorderVisitor::Visit(ExpressionList* expression_list) {
  auto new_expression_list = new ExpressionList();
  
  for (Expression* expression : expression_list->expressions_) {
    new_expression_list->Add(Accept(expression).expression_);
  }
  
  tos_value_.expression_list_ = new_expression_list;
}

void BlockBorderVisitor::Visit(NameExpression* name_expression) {
  tos_value_.expression_ = new NameExpression(name_expression->label_);
}

void BlockBorderVisitor::Visit(EseqExpression* eseq_expression) {
  IrtStorage child_statement = Accept(eseq_expression->statement_);
  IrtStorage child_expression = Accept(eseq_expression->expression_);
  
  tos_value_.expression_ = new EseqExpression(
      child_statement.statement_,
      child_expression.expression_
  );
}

Statement* BlockBorderVisitor::GetTree() {
  return new SeqStatement(
      tos_value_.statement_,
      new SeqStatement(
          new JumpStatement(Label(method_name_ + "_done")),
          new LabelStatement(Label(method_name_ + "_done"))
      )
  );
}

}