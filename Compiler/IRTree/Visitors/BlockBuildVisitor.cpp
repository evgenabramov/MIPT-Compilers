#include "BlockBuildVisitor.h"

#include "BaseElements.h"

#include "../Blocks/ConditionalBlock.hpp"
#include "../Blocks/SimpleBlock.hpp"

namespace irt {

void BlockBuildVisitor::Visit(ExpStatement* statement) {
  statement->GetExpression()->Accept(this);
}

void BlockBuildVisitor::Visit(ConstExpression* const_expression) {}

void BlockBuildVisitor::Visit(JumpConditionalStatement* jump_conditional_statement) {
  blocks_[current_label_.ToString()] = new ConditionalBlock(
      current_label_,
      current_statement_,
      jump_conditional_statement->label_true_,
      jump_conditional_statement->label_false_
  );
  
  if (root_ == nullptr) {
    root_ = blocks_[current_label_.ToString()];
  }
  
  jump_conditional_statement->left_operand_->Accept(this);
  jump_conditional_statement->right_operand_->Accept(this);
}

void BlockBuildVisitor::Visit(MoveStatement* move_statement) {
  move_statement->target_->Accept(this);
  move_statement->source_->Accept(this);
}

void BlockBuildVisitor::Visit(SeqStatement* seq_statement) {
  if (seq_statement->first_statement_->IsLabelStatement()) {
    current_label_ = dynamic_cast<LabelStatement*>(seq_statement->first_statement_)->label_;
    current_statement_ = seq_statement->second_statement_;
  }
  seq_statement->first_statement_->Accept(this);
  seq_statement->second_statement_->Accept(this);
}

void BlockBuildVisitor::Visit(LabelStatement* label_statement) {}

void BlockBuildVisitor::Visit(BinopExpression* binop_expression) {
  binop_expression->first_->Accept(this);
  binop_expression->second_->Accept(this);
}

void BlockBuildVisitor::Visit(TempExpression* temp_expression) {}

void BlockBuildVisitor::Visit(MemExpression* mem_expression) {
  mem_expression->expression_->Accept(this);
}

void BlockBuildVisitor::Visit(JumpStatement* jump_statement) {
  blocks_[current_label_.ToString()] = new SimpleBlock(
      current_label_,
      current_statement_,
      jump_statement->label_
  );
  
  if (root_ == nullptr) {
    root_ = blocks_[current_label_.ToString()];
  }
}

void BlockBuildVisitor::Visit(CallExpression* call_expression) {
  call_expression->function_name_->Accept(this);
  call_expression->args_->Accept(this);
}

void BlockBuildVisitor::Visit(ExpressionList* expression_list) {
  for (auto expression: expression_list->expressions_) {
    expression->Accept(this);
  }
}

void BlockBuildVisitor::Visit(NameExpression* name_expression) {}

void BlockBuildVisitor::Visit(EseqExpression* eseq_expression) {
  eseq_expression->statement_->Accept(this);
  eseq_expression->expression_->Accept(this);
}

BlockGraph BlockBuildVisitor::GetBlockGraph() {
  for (auto&[label, block] : blocks_) {
    if (block == nullptr) {
      continue;
    }
    
    if (block->IsSimple()) {
      auto simple_block = dynamic_cast<SimpleBlock*>(block);
      
      simple_block->SetNextBlock(blocks_[simple_block->GetNextBlockLabel().ToString()]);
    } else {
      auto conditional_block = dynamic_cast<ConditionalBlock*>(block);
      
      conditional_block->SetNextBlocks(
          blocks_[conditional_block->GetTrueBlockLabel().ToString()],
          blocks_[conditional_block->GetFalseBlockLabel().ToString()]
      );
    }
  }
  return BlockGraph(root_);
}

}