#include "PrintVisitor.h"

#include "BaseElements.h"

namespace irt {

PrintVisitor::PrintVisitor(const std::string& method_name, const std::string& suffix) : stream_(method_name + suffix) {
  stream_ << "Method tree for " + method_name << ":" << std::endl;
}

PrintVisitor::~PrintVisitor() {
  stream_.close();
}

void PrintVisitor::Visit(ExpStatement* statement) {
  PrintTabs();
  stream_ << "ExpStatement:" << std::endl;
  
  ++num_tabs_;
  statement->GetExpression()->Accept(this);
  --num_tabs_;
}

void PrintVisitor::Visit(ConstExpression* const_expression) {
  PrintTabs();
  stream_ << "ConstExpression " << const_expression->Value() << std::endl;
}

void PrintVisitor::Visit(JumpConditionalStatement* jump_conditional_statement) {
  PrintTabs();
  stream_ << "JumpConditionalStatement: " <<
          ToString(jump_conditional_statement->operator_type_) << std::endl;
  PrintTabs();
  stream_ << "TrueLabel: " << jump_conditional_statement->label_true_.ToString() << std::endl;
  PrintTabs();
  stream_ << "FalseLabel: " << jump_conditional_statement->label_false_.ToString() << std::endl;
  ++num_tabs_;
  jump_conditional_statement->left_operand_->Accept(this);
  jump_conditional_statement->right_operand_->Accept(this);
  --num_tabs_;
}

void PrintVisitor::Visit(MoveStatement* move_statement) {
  PrintTabs();
  stream_ << "MoveStatement:" << std::endl;
  ++num_tabs_;
  move_statement->target_->Accept(this);
  move_statement->source_->Accept(this);
  --num_tabs_;
}

void PrintVisitor::Visit(SeqStatement* seq_statement) {
  PrintTabs();
  stream_ << "SeqStatement:" << std::endl;
  ++num_tabs_;
  seq_statement->first_statement_->Accept(this);
  seq_statement->second_statement_->Accept(this);
  --num_tabs_;
  
}

void PrintVisitor::Visit(LabelStatement* label_statement) {
  PrintTabs();
  stream_ << "LabelStatement: " << label_statement->label_.ToString() << std::endl;
}

void PrintVisitor::Visit(BinopExpression* binop_expression) {
  PrintTabs();
  stream_ << "BinopExpression: " << ToString(binop_expression->operator_type_) << std::endl;
  ++num_tabs_;
  binop_expression->first_->Accept(this);
  binop_expression->second_->Accept(this);
  --num_tabs_;
}

void PrintVisitor::Visit(TempExpression* temp_expression) {
  PrintTabs();
  stream_ << "TempExpression: " << temp_expression->temporary_.ToString() << std::endl;
}

void PrintVisitor::Visit(MemExpression* mem_expression) {
  PrintTabs();
  stream_ << "MemExpression: " << std::endl;
  ++num_tabs_;
  mem_expression->expression_->Accept(this);
  --num_tabs_;
}

void PrintVisitor::Visit(JumpStatement* jump_statement) {
  PrintTabs();
  stream_ << "JumpStatement: " << jump_statement->label_.ToString() << std::endl;
}

void PrintVisitor::Visit(CallExpression* call_expression) {
  PrintTabs();
  stream_ << "CallExpression: " << std::endl;
  ++num_tabs_;
  call_expression->function_name_->Accept(this);
  call_expression->args_->Accept(this);
  --num_tabs_;
}

void PrintVisitor::Visit(ExpressionList* expression_list) {
  PrintTabs();
  stream_ << "ExpressionList: " << std::endl;
  ++num_tabs_;
  for (auto expression: expression_list->expressions_) {
    expression->Accept(this);
  }
  --num_tabs_;
}

void PrintVisitor::Visit(NameExpression* name_expression) {
  PrintTabs();
  stream_ << "NameExpression: " << name_expression->label_.ToString() << std::endl;
}

void PrintVisitor::Visit(EseqExpression* eseq_expression) {
  PrintTabs();
  stream_ << "EseqExpression:" << std::endl;
  ++num_tabs_;
  eseq_expression->statement_->Accept(this);
  eseq_expression->expression_->Accept(this);
  --num_tabs_;
}

void PrintVisitor::PrintTabs() {
  for (int i = 0; i < num_tabs_; ++i) {
    stream_ << '\t';
  }
}

void PrintVisitor::ChangeStream(const std::string& method_name, const std::string& suffix) {
  stream_ = std::move(std::ofstream(method_name + suffix));
  num_tabs_ = 0;
  stream_ << "Method tree for " + method_name << ":" << std::endl;
}

}