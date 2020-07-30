#pragma once

#include "Visitor.h"

#include <string>
#include <fstream>

namespace irt {

class PrintVisitor : public Visitor {

 public:
  PrintVisitor(const std::string& method_name, const std::string& suffix);
  ~PrintVisitor() override;

  void Visit(ExpStatement* statement) override;
  void Visit(ConstExpression* const_expression) override;
  void Visit(JumpConditionalStatement* jump_conditional_statement) override;
  void Visit(MoveStatement* move_statement) override;
  void Visit(SeqStatement* seq_statement) override;
  void Visit(LabelStatement* label_statement) override;
  void Visit(BinopExpression* binop_expression) override;
  void Visit(TempExpression* temp_exression) override;
  void Visit(MemExpression* mem_expression) override;
  void Visit(JumpStatement* jump_statement) override;
  void Visit(CallExpression* call_expression) override;
  void Visit(ExpressionList* expression_list) override;
  void Visit(NameExpression* name_expression) override;
  void Visit(EseqExpression* eseq_expression) override;
  
  void ChangeStream(const std::string& method_name, const std::string& suffix);

 private:
  void PrintTabs();

  std::ofstream stream_;
  int num_tabs_ = 0;
};

}

