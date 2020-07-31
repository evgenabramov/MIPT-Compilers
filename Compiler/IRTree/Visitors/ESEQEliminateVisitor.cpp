#include <cassert>
#include "ESEQEliminateVisitor.h"

#include "BaseElements.h"

namespace irt {

void ESEQEliminateVisitor::Visit(ExpStatement* statement) {
  IrtStorage child_expression = Accept(statement->GetExpression());

  if (child_expression.expression_->IsEseqExpression()) {
    auto eseq_child_expression = dynamic_cast<EseqExpression*>(child_expression.expression_);

    tos_value_.statement_ = eseq_child_expression->statement_;
  } else {
    tos_value_.statement_ = new ExpStatement(child_expression.expression_);
  }
}

void ESEQEliminateVisitor::Visit(ConstExpression* const_expression) {
  tos_value_.expression_ = new ConstExpression(const_expression->Value());
}

void ESEQEliminateVisitor::Visit(JumpConditionalStatement* jump_conditional_statement) {
  auto operator_type = jump_conditional_statement->operator_type_;
  auto label_true = jump_conditional_statement->label_true_;
  auto label_false = jump_conditional_statement->label_false_;
  Expression* lhs = Accept(jump_conditional_statement->left_operand_).expression_;
  Expression* rhs = Accept(jump_conditional_statement->right_operand_).expression_;

  if (lhs->IsEseqExpression() && rhs->IsEseqExpression()) {
    auto eseq_lhs = dynamic_cast<EseqExpression*>(lhs);
    auto eseq_rhs = dynamic_cast<EseqExpression*>(rhs);

    if (!CheckCommute(eseq_rhs->statement_, eseq_lhs->expression_)) {
      Temporary temp;

      tos_value_.statement_ = new SeqStatement(
          eseq_lhs->statement_,
          new SeqStatement(
              new MoveStatement(
                  new TempExpression(temp),
                  eseq_lhs->expression_
              ),
              new SeqStatement(
                  eseq_rhs->statement_,
                  new JumpConditionalStatement(
                      operator_type,
                      new TempExpression(temp),
                      eseq_rhs->expression_,
                      label_true,
                      label_false
                  )
              )
          )
      );
    } else {
      tos_value_.statement_ = new SeqStatement(
          eseq_lhs->statement_,
          new SeqStatement(
              eseq_rhs->statement_,
              new JumpConditionalStatement(
                  operator_type,
                  eseq_lhs->expression_,
                  eseq_rhs->expression_,
                  label_true,
                  label_false
              )
          )
      );
    }
  } else if (lhs->IsEseqExpression()) {
    auto eseq_lhs = dynamic_cast<EseqExpression*>(lhs);

    tos_value_.statement_ = new SeqStatement(
        eseq_lhs->statement_,
        new JumpConditionalStatement(
            operator_type,
            eseq_lhs->expression_,
            rhs,
            label_true,
            label_false
        )
    );
  } else if (rhs->IsEseqExpression()) {
    auto eseq_rhs = dynamic_cast<EseqExpression*>(rhs);

    if (!CheckCommute(eseq_rhs->statement_, lhs)) {
      Temporary temp;

      tos_value_.statement_ = new SeqStatement(
          new MoveStatement(
              new TempExpression(temp),
              lhs
          ),
          new SeqStatement(
              eseq_rhs->statement_,
              new JumpConditionalStatement(
                  operator_type,
                  new TempExpression(temp),
                  eseq_rhs->expression_,
                  label_true,
                  label_false
              )
          )
      );
    } else {
      tos_value_.statement_ = new SeqStatement(
          eseq_rhs->statement_,
          new JumpConditionalStatement(
              operator_type,
              lhs,
              eseq_rhs->expression_,
              label_true,
              label_false
          )
      );
    }
  } else {
    tos_value_.statement_ = new JumpConditionalStatement(
        operator_type,
        lhs,
        rhs,
        label_true,
        label_false
    );
  }
}

void ESEQEliminateVisitor::Visit(MoveStatement* move_statement) {
  IrtStorage target = Accept(move_statement->target_);
  IrtStorage source = Accept(move_statement->source_);

  if (source.expression_->IsEseqExpression() && target.expression_->IsEseqExpression()) {
    auto source_eseq_expression = dynamic_cast<EseqExpression*>(source.expression_);
    auto target_eseq_expression = dynamic_cast<EseqExpression*>(target.expression_);

    tos_value_.statement_ = new SeqStatement(
        source_eseq_expression->statement_,
        new SeqStatement(
            target_eseq_expression->statement_,
            new MoveStatement(target_eseq_expression->expression_, source_eseq_expression->expression_)
        )
    );
  } else if (source.expression_->IsEseqExpression()) {
    auto source_eseq_expression = dynamic_cast<EseqExpression*>(source.expression_);

    tos_value_.statement_ = new SeqStatement(
        source_eseq_expression->statement_,
        new MoveStatement(target.expression_, source_eseq_expression->expression_)
    );
  } else if (target.expression_->IsEseqExpression()) {
    auto target_eseq_expression = dynamic_cast<EseqExpression*>(target.expression_);

    tos_value_.statement_ = new SeqStatement(
        target_eseq_expression->statement_,
        new MoveStatement(target_eseq_expression->expression_, source.expression_)
    );
  } else {
    tos_value_.statement_ = new MoveStatement(target.expression_, source.expression_);
  }
}

void ESEQEliminateVisitor::Visit(SeqStatement* seq_statement) {
  IrtStorage first = Accept(seq_statement->first_statement_);
  IrtStorage second = Accept(seq_statement->second_statement_);

  tos_value_.statement_ = new SeqStatement(
      first.statement_,
      second.statement_
  );
}

void ESEQEliminateVisitor::Visit(LabelStatement* label_statement) {
  tos_value_.statement_ = new LabelStatement(label_statement->label_);
}

void ESEQEliminateVisitor::Visit(BinopExpression* binop_expression) {
  auto operator_type = binop_expression->operator_type_;
  Expression* lhs = Accept(binop_expression->first_).expression_;
  Expression* rhs = Accept(binop_expression->second_).expression_;

  if (lhs->IsEseqExpression() && rhs->IsEseqExpression()) {
    auto eseq_lhs = dynamic_cast<EseqExpression*>(lhs);
    auto eseq_rhs = dynamic_cast<EseqExpression*>(rhs);

    if (!CheckCommute(eseq_rhs->statement_, eseq_lhs->expression_)) {
      Temporary temp;

      tos_value_.expression_ = new EseqExpression(
          new SeqStatement(
              eseq_lhs->statement_,
              new SeqStatement(
                  new MoveStatement(
                      new TempExpression(temp),
                      eseq_lhs->expression_
                  ),
                  eseq_rhs->statement_
              )
          ),
          new BinopExpression(
              operator_type,
              new TempExpression(temp),
              eseq_rhs->expression_
          )
      );
    } else {
      tos_value_.expression_ = new EseqExpression(
          new SeqStatement(
              eseq_lhs->statement_,
              eseq_rhs->statement_
          ),
          new BinopExpression(
              operator_type,
              eseq_lhs->expression_,
              eseq_rhs->expression_
          )
      );
    }
  } else if (lhs->IsEseqExpression()) {
    auto eseq_lhs = dynamic_cast<EseqExpression*>(lhs);

    tos_value_.expression_ = new EseqExpression(
        eseq_lhs->statement_,
        new BinopExpression(
            operator_type,
            eseq_lhs->expression_,
            rhs
        )
    );
  } else if (rhs->IsEseqExpression()) {
    auto eseq_rhs = dynamic_cast<EseqExpression*>(rhs);

    if (!CheckCommute(eseq_rhs->statement_, lhs)) {
      Temporary temp;

      tos_value_.expression_ = new EseqExpression(
          new SeqStatement(
              new MoveStatement(
                  new TempExpression(temp),
                  lhs
              ),
              eseq_rhs->statement_
          ),
          new BinopExpression(
                  operator_type,
                  new TempExpression(temp),
                  eseq_rhs->expression_
          )
      );
    } else {
      tos_value_.expression_ = new EseqExpression(
          eseq_rhs->statement_,
          new BinopExpression(
              operator_type,
              lhs,
              eseq_rhs->expression_
          )
      );
    }
  } else {
    tos_value_.expression_ = new BinopExpression(
        operator_type,
        lhs,
        rhs
    );
  }
}

void ESEQEliminateVisitor::Visit(TempExpression* temp_expression) {
  tos_value_.expression_ = new TempExpression(temp_expression->temporary_);
}

void ESEQEliminateVisitor::Visit(MemExpression* mem_expression) {
  IrtStorage child_expression = Accept(mem_expression->expression_);

  if (child_expression.expression_->IsEseqExpression()) {
    auto eseq_expression = dynamic_cast<EseqExpression*>(child_expression.expression_);

    tos_value_.expression_ = new EseqExpression(
        eseq_expression->statement_,
        new MemExpression(eseq_expression->expression_)
    );
  } else {
    tos_value_.expression_ = new MemExpression(child_expression.expression_);
  }
}

void ESEQEliminateVisitor::Visit(JumpStatement* jump_statement) {
  tos_value_.statement_ = new JumpStatement(jump_statement->label_);
}

void ESEQEliminateVisitor::Visit(CallExpression* call_expression) {
  IrtStorage func = Accept(call_expression->function_name_);
  IrtStorage args = Accept(call_expression->args_);

  int first_eseq_index = -1;
  for (size_t i = 0; i < args.expression_list_->expressions_.size(); ++i) {
    if (args.expression_list_->expressions_[i]->IsEseqExpression()) {
      first_eseq_index = i;
      break;
    }
  }

  if (first_eseq_index == -1) {
    if (func.expression_->IsEseqExpression()) {
      auto eseq_func_expression = dynamic_cast<EseqExpression*>(func.expression_);

      tos_value_.expression_ = new EseqExpression(
          eseq_func_expression->statement_,
          new CallExpression(
              eseq_func_expression->expression_,
              args.expression_list_
          )
      );
    } else {
      tos_value_.expression_ = new CallExpression(
          func.expression_,
          args.expression_list_
      );
    }
    return;
  }

  auto eseq_argument = dynamic_cast<EseqExpression*>(args.expression_list_->expressions_[first_eseq_index]);

  std::vector<Temporary> temps;
  std::vector<int> noncommute_indices(1, -1);
  for (int i = first_eseq_index - 1; i >= 0; --i) {
    if (!CheckCommute(eseq_argument->statement_, args.expression_list_->expressions_[i])) {
      noncommute_indices.push_back(i);
      temps.emplace_back();
    }
  }

  // Add temporaries to save arguments
  Statement* suffix_statement = eseq_argument->statement_;
  for (int i = static_cast<int>(temps.size()) - 1; i >= 0; --i) {
    size_t index = noncommute_indices[i + 1];
    suffix_statement = new SeqStatement(
        new MoveStatement(
            new TempExpression(temps[i]),
            args.expression_list_->expressions_[index]
        ),
        suffix_statement
    );
  }

  // Construct new ExpressionList
  auto new_expression_list = new ExpressionList();
  for (int i = 1; i < noncommute_indices.size(); ++i) {
    size_t noncommute_index = noncommute_indices[i];
    size_t prev_index = noncommute_indices[i - 1];
    for (size_t index = prev_index + 1; index < noncommute_index; ++index) {
      new_expression_list->Add(args.expression_list_->expressions_[index]);
    }
    new_expression_list->Add(new TempExpression(temps[i - 1]));
  }
  for (size_t i = noncommute_indices.back() + 1; i < args.expression_list_->expressions_.size(); ++i) {
    if (i == first_eseq_index) {
      new_expression_list->Add(eseq_argument->expression_);
    } else {
      new_expression_list->Add(args.expression_list_->expressions_[i]);
    }
  }
  assert(call_expression->args_->expressions_.size() == new_expression_list->expressions_.size());

  if (func.expression_->IsEseqExpression()) {
    auto eseq_func_expression = dynamic_cast<EseqExpression*>(func.expression_);

    // Replace other ESEQ arguments
    IrtStorage new_call_expression = Accept(
        new CallExpression(
            eseq_func_expression->expression_,
            new_expression_list
        )
    );

    suffix_statement = new SeqStatement(
        eseq_func_expression->statement_,
        suffix_statement
    );

    tos_value_.expression_ = new EseqExpression(suffix_statement, new_call_expression.expression_);
  } else {
    // Replace other ESEQ arguments
    IrtStorage new_call_expression = Accept(
        new CallExpression(
            func.expression_,
            new_expression_list
        )
    );

    tos_value_.expression_ = new EseqExpression(suffix_statement, new_call_expression.expression_);
  }
}

void ESEQEliminateVisitor::Visit(ExpressionList* expression_list) {
  auto new_expression_list = new ExpressionList();

  for (Expression* expression : expression_list->expressions_) {
    new_expression_list->Add(Accept(expression).expression_);
  }

  tos_value_.expression_list_ = new_expression_list;
}

void ESEQEliminateVisitor::Visit(NameExpression* name_expression) {
  tos_value_.expression_ = new NameExpression(name_expression->label_);
}

void ESEQEliminateVisitor::Visit(EseqExpression* eseq_expression) {
  IrtStorage child_statement = Accept(eseq_expression->statement_);
  IrtStorage child_expression = Accept(eseq_expression->expression_);

  if (child_expression.expression_->IsEseqExpression()) {
    auto eseq_child_expression = dynamic_cast<EseqExpression*>(child_expression.expression_);

    tos_value_.expression_ = new EseqExpression(
        new SeqStatement(child_statement.statement_, eseq_child_expression->statement_),
        eseq_child_expression->expression_
    );
  } else {
    tos_value_.expression_ = new EseqExpression(
        child_statement.statement_,
        child_expression.expression_
    );
  }
}

Statement* ESEQEliminateVisitor::GetTree() {
  return tos_value_.statement_;
}

bool ESEQEliminateVisitor::CheckCommute(Statement* statement, Expression* expression) {
  return ((statement->IsExpStatement() && dynamic_cast<ExpStatement*>(statement)->GetExpression())
      || expression->IsNameExpression() || expression->IsConstExpression());
}

}