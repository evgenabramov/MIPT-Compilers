#pragma once

#include "forward_decl.hpp"

namespace irt {

class Visitor {
 public:
  virtual ~Visitor() = default;

  virtual void Visit(ExpStatement* stmt) = 0;
  virtual void Visit(ConstExpression* const_expression) = 0;
  virtual void Visit(JumpConditionalStatement* jump_conditional_statement) = 0;
  virtual void Visit(MoveStatement* move_statement) = 0;
  virtual void Visit(SeqStatement* seq_statement) = 0;
  virtual void Visit(LabelStatement* label_statement) = 0;
  virtual void Visit(BinopExpression* binop_statement) = 0;
  virtual void Visit(TempExpression* temp_exression) = 0;
  virtual void Visit(MemExpression* mem_expression) = 0;
  virtual void Visit(JumpStatement* jump_statement) = 0;
  virtual void Visit(CallExpression* call_expression) = 0;
  virtual void Visit(ExpressionList* expression_list) = 0;
  virtual void Visit(NameExpression* name_expression) = 0;
  virtual void Visit(EseqExpression* eseq_expression) = 0;
};

}
