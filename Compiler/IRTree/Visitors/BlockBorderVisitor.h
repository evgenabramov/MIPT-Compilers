#pragma once

#include "TemplateVisitor.hpp"
#include "VisitorStruct.h"

namespace irt {

// Prepare IRTree for blocks building
class BlockBorderVisitor : public TemplateVisitor<IrtStorage> {
 public:
  void Visit(ExpStatement* statement) override;
  void Visit(ConstExpression* const_expression) override;
  void Visit(JumpConditionalStatement* jump_conditional_statement) override;
  void Visit(MoveStatement* move_statement) override;
  void Visit(SeqStatement* seq_statement) override;
  void Visit(LabelStatement* label_statement) override;
  void Visit(BinopExpression* binop_expression) override;
  void Visit(TempExpression* temp_expression) override;
  void Visit(MemExpression* mem_expression) override;
  void Visit(JumpStatement* jump_statement) override;
  void Visit(CallExpression* call_expression) override;
  void Visit(ExpressionList* expression_list) override;
  void Visit(NameExpression* name_expression) override;
  void Visit(EseqExpression* eseq_expression) override;
  Statement* GetTree();
  
 private:
  bool is_block_finished_ = true;
};

}