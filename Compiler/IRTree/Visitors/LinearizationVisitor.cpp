#include "LinearizationVisitor.h"

#include "BaseElements.h"

namespace irt {

void LinearizationVisitor::Visit(ExpStatement* statement) {
  IrtStorage child_expression = Accept(statement->GetExpression());
  tos_value_.statement_ = new ExpStatement(child_expression.expression_);
}

void LinearizationVisitor::Visit(ConstExpression* const_expression) {
  tos_value_.expression_ = new ConstExpression(const_expression->Value());
}

void LinearizationVisitor::Visit(JumpConditionalStatement* jump_conditional_statement) {
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

void LinearizationVisitor::Visit(MoveStatement* move_statement) {
  IrtStorage target = Accept(move_statement->target_);
  IrtStorage source = Accept(move_statement->source_);

  tos_value_.statement_ = new MoveStatement(
      target.expression_,
      source.expression_
  );
}

void LinearizationVisitor::Visit(SeqStatement* seq_statement) {
  IrtStorage first = Accept(seq_statement->first_statement_);
  IrtStorage second = Accept(seq_statement->second_statement_);

  if (first.statement_->IsSeqStatement()) {
    auto seq_first_statement = dynamic_cast<SeqStatement*>(first.statement_);

    tos_value_ = Accept(new SeqStatement(
        seq_first_statement->first_statement_,
        new SeqStatement(
            seq_first_statement->second_statement_,
            second.statement_
        )
    ));
  } else {
    tos_value_.statement_ = new SeqStatement(first.statement_, second.statement_);
  }
}

void LinearizationVisitor::Visit(LabelStatement* label_statement) {
  tos_value_.statement_ = new LabelStatement(label_statement->label_);
}

void LinearizationVisitor::Visit(BinopExpression* binop_expression) {
  IrtStorage first = Accept(binop_expression->first_);
  IrtStorage second = Accept(binop_expression->second_);

  tos_value_.expression_ = new BinopExpression(
      binop_expression->operator_type_,
      first.expression_,
      second.expression_
  );
}

void LinearizationVisitor::Visit(TempExpression* temp_expression) {
  tos_value_.expression_ = new TempExpression(temp_expression->temporary_);
}

void LinearizationVisitor::Visit(MemExpression* mem_expression) {
  IrtStorage child = Accept(mem_expression->expression_);

  tos_value_.expression_ = new MemExpression(child.expression_);
}

void LinearizationVisitor::Visit(JumpStatement* jump_statement) {
  tos_value_.statement_ = new JumpStatement(jump_statement->label_);
}

void LinearizationVisitor::Visit(CallExpression* call_expression) {
  IrtStorage func = Accept(call_expression->function_name_);
  IrtStorage args = Accept(call_expression->args_);

  tos_value_.expression_ = new CallExpression(
      func.expression_,
      args.expression_list_
  );
}

void LinearizationVisitor::Visit(ExpressionList* expression_list) {
  auto new_expression_list = new ExpressionList();

  for (Expression* expression : expression_list->expressions_) {
    new_expression_list->Add(Accept(expression).expression_);
  }

  tos_value_.expression_list_ = new_expression_list;
}

void LinearizationVisitor::Visit(NameExpression* name_expression) {
  tos_value_.expression_ = new NameExpression(name_expression->label_);
}

void LinearizationVisitor::Visit(EseqExpression* eseq_expression) {
  IrtStorage child_statement = Accept(eseq_expression->statement_);
  IrtStorage child_expression = Accept(eseq_expression->expression_);

  tos_value_.expression_ = new EseqExpression(
      child_statement.statement_,
      child_expression.expression_
  );
}

Statement* LinearizationVisitor::GetTree() {
  return tos_value_.statement_;
}

}
