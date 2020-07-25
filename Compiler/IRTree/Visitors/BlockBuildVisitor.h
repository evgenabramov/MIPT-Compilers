#pragma once

#include "TemplateVisitor.hpp"
#include "VisitorStruct.h"
#include "../Blocks/BlockGraph.hpp"

#include <unordered_map>

namespace irt {

class BlockBuildVisitor : public Visitor {
 public:
  void Visit(ExpStatement* statement) override;
  void Visit(ConstExpression* const_expression) override;
  void Visit(JumpConditionalStatement* jump_conditional_statement) override;
  void Visit(MoveStatement* move_statement) override;
  void Visit(SeqStatement* seq_statement) override;
  void Visit(LabelStatement* label_statement) override;
  void Visit(BinopExpression* binop_statement) override;
  void Visit(TempExpression* temp_expression) override;
  void Visit(MemExpression* mem_expression) override;
  void Visit(JumpStatement* jump_statement) override;
  void Visit(CallExpression* call_expression) override;
  void Visit(ExpressionList* expression_list) override;
  void Visit(NameExpression* name_expression) override;
  void Visit(EseqExpression* eseq_expression) override;
  
  BlockGraph GetBlockGraph();
  
 private:
  Label current_label_;
  Statement* current_statement_ = nullptr;
  
  Block* root_ = nullptr;
  
  std::unordered_map<std::string, Block*> blocks_;
};

}