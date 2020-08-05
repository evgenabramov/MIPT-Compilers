#pragma once

#include "Visitor.h"
#include "TemplateVisitor.hpp"
#include "../Generators/Temporary.h"
#include "../Instructions/Instruction.hpp"

#include <vector>

namespace irt {

class CodeGenerationVisitor : public TemplateVisitor<Temporary> {
 public:
  CodeGenerationVisitor(size_t num_arguments, size_t frame_size);
  
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
  
  std::vector<Instruction> GetInstructions() const;
  
  void PrintInstructions(const std::string& filename) const;
 
 private:
  void EmitCallExpression(const Temporary& result_temp, CallExpression* call_expression);
  
  void Emit(std::string str, std::vector<Temporary> targets, std::vector<Temporary> sources);
  
  std::vector<Instruction> instructions_;
  
  size_t num_arguments_;
  size_t frame_size_;
};

}