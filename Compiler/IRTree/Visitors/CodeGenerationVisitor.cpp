#include "CodeGenerationVisitor.h"

#include "BaseElements.h"

namespace irt {

CodeGenerationVisitor::CodeGenerationVisitor(size_t num_arguments, size_t frame_size)
    : num_arguments_(num_arguments), frame_size_(frame_size) {}

void CodeGenerationVisitor::Visit(ExpStatement* statement) {
  statement->GetExpression()->Accept(this);
}

void CodeGenerationVisitor::Visit(ConstExpression* const_expression) {
  // The worst case when we have to save a const to register
  Temporary result_temp;
  Emit(
      "mov t0, #" + std::to_string(const_expression->Value()),
      {result_temp},
      {}
  );
  tos_value_ = result_temp;
}

void CodeGenerationVisitor::Visit(JumpConditionalStatement* jump_conditional_statement) {
  LogicOperatorType operator_type = jump_conditional_statement->operator_type_;
  
  if (jump_conditional_statement->right_operand_->IsConstExpression()) {
    // JUMP_COND(OP. e1, CONST(i), LABEL_TRUE, LABEL_FALSE)
    
    auto const_rhs = dynamic_cast<ConstExpression*>(jump_conditional_statement->right_operand_);
    Temporary lhs_temp = Accept(jump_conditional_statement->left_operand_);
    
    Emit(
        "cmp s0, #" + std::to_string(const_rhs->Value()),
        {},
        {lhs_temp}
    );
  } else if (jump_conditional_statement->left_operand_->IsConstExpression()) {
    // JUMP_COND(OP, CONST(i), e1, LABEL_TRUE, LABEL_FALSE)
    
    auto const_lhs = dynamic_cast<ConstExpression*>(jump_conditional_statement->left_operand_);
    Temporary rhs_temp = Accept(jump_conditional_statement->right_operand_);
    
    // Reverse logic operator
    switch (jump_conditional_statement->operator_type_) {
      case LogicOperatorType::EQ:operator_type = LogicOperatorType::NE;
        break;
      case LogicOperatorType::NE:operator_type = LogicOperatorType::EQ;
        break;
      case LogicOperatorType::GE:operator_type = LogicOperatorType::LE;
        break;
      case LogicOperatorType::GT:operator_type = LogicOperatorType::LT;
        break;
      case LogicOperatorType::LE:operator_type = LogicOperatorType::GE;
        break;
      case LogicOperatorType::LT:operator_type = LogicOperatorType::GT;
        break;
    }
    
    Emit(
        "cmp s0, #" + std::to_string(const_lhs->Value()),
        {},
        {rhs_temp}
    );
  } else {
    // JUMP_COND(OP, e1, e2, LABEL_TRUE, LABEL_FALSE) - trivial case
    
    Temporary lhs_temp = Accept(jump_conditional_statement->left_operand_);
    Temporary rhs_temp = Accept(jump_conditional_statement->right_operand_);
    
    Emit("cmp s0, s1", {}, {lhs_temp, rhs_temp});
  }
  
  std::string instruction;
  
  switch (operator_type) {
    case LogicOperatorType::EQ:instruction = "beq";
      break;
    case LogicOperatorType::NE:instruction = "bne";
      break;
    case LogicOperatorType::GE:instruction = "bge";
      break;
    case LogicOperatorType::GT:instruction = "bgt";
      break;
    case LogicOperatorType::LE:instruction = "ble";
      break;
    case LogicOperatorType::LT:instruction = "blt";
      break;
  }
  
  instruction += " " + jump_conditional_statement->label_true_.ToString();
  
  Emit(instruction, {}, {});
  Emit("b " + jump_conditional_statement->label_false_.ToString(), {}, {});
}

void CodeGenerationVisitor::Visit(MoveStatement* move_statement) {
  Expression* target = move_statement->target_;
  Expression* source = move_statement->source_;
  
  if (target->IsMemExpression()) {
    // MOVE(MEM(e1), e2)
    
    auto mem_target = dynamic_cast<MemExpression*>(target);
    Temporary source_temp = Accept(move_statement->source_);
    
    if (mem_target->expression_->IsBinopExpression() && !source->IsMemExpression()) {
      // MOVE(MEM(BINOP), e1)
      
      auto binop = dynamic_cast<BinopExpression*>(mem_target->expression_);
      
      if (binop->operator_type_ == BinaryOperatorType::PLUS) {
        // MOVE(MEM(BINOP(PLUS, e1, e2)), e3))
        
        if (binop->first_->IsConstExpression()) {
          // MOVE(MEM(BINOP(PLUS, CONST(i), e1)), e2)
          
          auto const_lhs = dynamic_cast<ConstExpression*>(binop->first_);
          Temporary rhs_temp = Accept(binop->second_);
          
          Emit(
              "str s0, [s1, #" + std::to_string(const_lhs->Value()) + "]",
              {},
              {source_temp, rhs_temp}
          );
        } else if (binop->second_->IsConstExpression()) {
          // MOVE(MEM(BINOP(PLUS, e1, CONST(i))), e2)
          
          Temporary lhs_temp = Accept(binop->first_);
          auto const_rhs = dynamic_cast<ConstExpression*>(binop->second_);
          
          Emit(
              "str s0, [s1, #" + std::to_string(const_rhs->Value()) + "]",
              {},
              {source_temp, lhs_temp}
          );
        } else {
          // MOVE(MEM(BINOP(PLUS, e1, e2)), e3) - trivial case
          
          Temporary lhs_temp = Accept(binop->first_);
          Temporary rhs_temp = Accept(binop->second_);
          
          Emit(
              "str s0, [s1, s2]",
              {},
              {source_temp, rhs_temp, lhs_temp}
          );
        }
      } else {
        // MOVE(MEM(BINOP), e1) - trivial case
        
        Temporary binop_temp = Accept(mem_target->expression_);
        
        Emit(
            "str s0, [s1]",
            {},
            {source_temp, binop_temp}
        );
      }
    } else if (source->IsMemExpression()) {
      // MOVE(MEM(e1), MEM(e2))
      
      auto second_statement = new MoveStatement(target, new TempExpression(source_temp));
      second_statement->Accept(this);
    } else {
      // MOVE(MEM(e1), e2) - trivial case
      
      Temporary target_temp = Accept(mem_target->expression_);
      
      Emit(
          "str s0, [s1]",
          {},
          {source_temp, target_temp}
      );
    }
  } else if (source->IsMemExpression()) {
    // MOVE(e1, MEM(e2))
    
    auto mem_source = dynamic_cast<MemExpression*>(source);
    Temporary target_temp = Accept(move_statement->target_);
    
    if (mem_source->expression_->IsBinopExpression()) {
      // MOVE(e1, MEM(BINOP))
      
      auto binop = dynamic_cast<BinopExpression*>(mem_source->expression_);
      
      if (binop->operator_type_ == BinaryOperatorType::PLUS) {
        // MOVE(e1, MEM(BINOP(PLUS, e2, e3))))
        
        if (binop->first_->IsConstExpression()) {
          // MOVE(e1, MEM(BINOP(PLUS, CONST(i), e2)))
          
          auto const_lhs = dynamic_cast<ConstExpression*>(binop->first_);
          Temporary rhs_temp = Accept(binop->second_);
          
          Emit(
              "ldr t0, [s0, #" + std::to_string(const_lhs->Value()) + "]",
              {target_temp},
              {rhs_temp}
          );
        } else if (binop->second_->IsConstExpression()) {
          // MOVE(e1, MEM(BINOP(PLUS, e2, CONST(i))))
          
          Temporary lhs_temp = Accept(binop->first_);
          auto const_rhs = dynamic_cast<ConstExpression*>(binop->second_);
          
          Emit(
              "ldr t0, [s0, #" + std::to_string(const_rhs->Value()) + "]",
              {target_temp},
              {lhs_temp}
          );
        } else {
          // MOVE(e1, BINOP(PLUS, e2, e3)) - trivial case
          
          Temporary lhs_temp = Accept(binop->first_);
          Temporary rhs_temp = Accept(binop->second_);
          
          Emit(
              "ldr t0, [s0, s1]",
              {target_temp},
              {lhs_temp, rhs_temp}
          );
        }
      } else {
        // MOVE(e1, MEM(BINOP)) - trivial case
        
        Temporary binop_temp = Accept(mem_source->expression_);
        
        Emit(
            "ldr t0, [s0]",
            {target_temp},
            {binop_temp}
        );
      }
    } else {
      // MOVE(e1, MEM(e2)) - trivial case
      
      Temporary source_temp = Accept(mem_source->expression_);
      
      Emit(
          "ldr t0, [s0]",
          {target_temp},
          {source_temp}
      );
    }
  } else if (source->IsConstExpression()) {
    // MOVE(e1, CONST(i))
    
    Temporary target_temp = Accept(target);
    auto const_source = dynamic_cast<ConstExpression*>(source);
    
    Emit(
        "mov t0, #" + std::to_string(const_source->Value()),
        {target_temp},
        {}
    );
  } else if (target->IsTempExpression() && source->IsCallExpression()) {
    auto temp_target = dynamic_cast<TempExpression*>(target);
    auto call_source = dynamic_cast<CallExpression*>(source);
    
    EmitCallExpression(temp_target->temporary_, call_source);
  } else {
    // MOVE(e1, e2) - trivial case
    
    Temporary target_temp = Accept(target);
    Temporary source_temp = Accept(source);
    
    Emit(
        "mov t0, s0",
        {target_temp},
        {source_temp}
    );
  }
}

void CodeGenerationVisitor::Visit(SeqStatement* seq_statement) {
  seq_statement->first_statement_->Accept(this);
  seq_statement->second_statement_->Accept(this);
}

void CodeGenerationVisitor::Visit(LabelStatement* label_statement) {
  Emit(label_statement->label_.ToString() + ":", {}, {});
}

void CodeGenerationVisitor::Visit(BinopExpression* binop_expression) {
  Temporary result_temp;
  
  bool is_commutative_operator = true;
  switch (binop_expression->operator_type_) {
    case BinaryOperatorType::PLUS:break;
    case BinaryOperatorType::MUL:break;
    case BinaryOperatorType::AND:break;
    case BinaryOperatorType::OR:break;
    default:is_commutative_operator = false;
  }
  
  if (binop_expression->first_->IsConstExpression() && binop_expression->second_->IsConstExpression()) {
    // BINOP(OP, CONST(i), CONST(j))
    
    auto const_lhs = dynamic_cast<ConstExpression*>(binop_expression->first_);
    auto const_rhs = dynamic_cast<ConstExpression*>(binop_expression->second_);
    
    switch (binop_expression->operator_type_) {
      case BinaryOperatorType::PLUS:
        Emit(
            "mov t0, #" + std::to_string(const_lhs->Value()) + "+" + std::to_string(const_rhs->Value()),
            {result_temp},
            {}
        );
        break;
      case BinaryOperatorType::MINUS:
        Emit(
            "mov t0, #" + std::to_string(const_lhs->Value()) + "-" + std::to_string(const_rhs->Value()),
            {result_temp},
            {}
        );
        break;
      case BinaryOperatorType::MUL:
        Emit(
            "mov t0, #" + std::to_string(const_lhs->Value()) + "*" + std::to_string(const_rhs->Value()),
            {result_temp},
            {}
        );
        break;
      case BinaryOperatorType::DIV:
        Emit(
            "mov t0, #" + std::to_string(const_lhs->Value()) + "/" + std::to_string(const_rhs->Value()),
            {result_temp},
            {}
        );
        break;
      case BinaryOperatorType::MOD:
        Emit(
            "mov t0, #" + std::to_string(const_lhs->Value()) + " mod " + std::to_string(const_rhs->Value()),
            {result_temp},
            {}
        );
        break;
      case BinaryOperatorType::AND:
        Emit(
            "mov t0, #(" + std::to_string(const_lhs->Value()) + " and " + std::to_string(const_rhs->Value()) + ")",
            {result_temp},
            {}
        );
        break;
      case BinaryOperatorType::OR:
        Emit(
            "mov t0, #(" + std::to_string(const_lhs->Value()) + " or " + std::to_string(const_rhs->Value()) + ")",
            {result_temp},
            {}
        );
        break;
    }
  } else if (binop_expression->second_->IsConstExpression()) {
    // BINOP(OP, e1, CONST(i))
    
    Temporary lhs_temp = Accept(binop_expression->first_);
    auto const_rhs = dynamic_cast<ConstExpression*>(binop_expression->second_);
    
    switch (binop_expression->operator_type_) {
      case BinaryOperatorType::PLUS:
        Emit(
            "add t0, s0, #" + std::to_string(const_rhs->Value()),
            {result_temp},
            {lhs_temp}
        );
        break;
      case BinaryOperatorType::MINUS:
        Emit(
            "sub t0, s0, #" + std::to_string(const_rhs->Value()),
            {result_temp},
            {lhs_temp}
        );
        break;
      case BinaryOperatorType::MUL:
        if (result_temp == lhs_temp) {
          Emit(
              "mul t0, #" + std::to_string(const_rhs->Value()),
              {result_temp},
              {lhs_temp}
          );
        } else {
          Temporary temp;
          Emit(
              "mov t0, #" + std::to_string(const_rhs->Value()),
              {temp},
              {}
          );
          Emit(
              "mul t0, s0, s1",
              {result_temp},
              {lhs_temp, temp}
          );
        }
        break;
      case BinaryOperatorType::DIV:
        Emit(
            "sdiv t0, s0, #" + std::to_string(const_rhs->Value()),
            {result_temp},
            {lhs_temp}
        );
        break;
      case BinaryOperatorType::MOD: {
        Temporary temp;
        Emit(
            "sdiv t0, s0, #" + std::to_string(const_rhs->Value()),
            {temp},
            {lhs_temp}
        );
        Emit(
            "mul t0, #" + std::to_string(const_rhs->Value()),
            {temp},
            {}
        );
        Emit(
            "sub t0, s0, s1",
            {result_temp},
            {lhs_temp, temp}
        );
        break;
      }
      case BinaryOperatorType::AND:
        Emit(
            "and t0, s0, #" + std::to_string(const_rhs->Value()),
            {result_temp},
            {lhs_temp}
        );
        break;
      case BinaryOperatorType::OR:
        Emit(
            "orr t0, s0, #" + std::to_string(const_rhs->Value()),
            {result_temp},
            {lhs_temp}
        );
        break;
    }
  } else if (binop_expression->first_->IsConstExpression() && is_commutative_operator) {
    // BINOP(CONST(i), e1)
    
    auto const_lhs = dynamic_cast<ConstExpression*>(binop_expression->first_);
    Temporary rhs_temp = Accept(binop_expression->second_);
    
    switch (binop_expression->operator_type_) {
      case BinaryOperatorType::PLUS:
        Emit(
            "add t0, s0, #" + std::to_string(const_lhs->Value()),
            {result_temp},
            {rhs_temp}
        );
        break;
      case BinaryOperatorType::MUL:
        if (result_temp == rhs_temp) {
          Emit(
              "mul t0, #" + std::to_string(const_lhs->Value()),
              {result_temp},
              {rhs_temp}
          );
        } else {
          Temporary temp;
          Emit(
              "mov t0, #" + std::to_string(const_lhs->Value()),
              {temp},
              {}
          );
          Emit(
              "mul t0, s0, s1",
              {result_temp},
              {rhs_temp, temp}
          );
        }
        break;
      case BinaryOperatorType::AND:
        Emit(
            "and t0, s0, #" + std::to_string(const_lhs->Value()),
            {result_temp},
            {rhs_temp}
        );
        break;
      case BinaryOperatorType::OR:
        Emit(
            "orr t0, s0, #" + std::to_string(const_lhs->Value()),
            {result_temp},
            {rhs_temp}
        );
        break;
      default:break;
    }
  } else {
    // BINOP(OP, e1, e2) - trivial case
    
    Temporary lhs_temp = Accept(binop_expression->first_);
    Temporary rhs_temp = Accept(binop_expression->second_);
    
    switch (binop_expression->operator_type_) {
      case BinaryOperatorType::PLUS:
        Emit(
            "add t0, s0, s1",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
        break;
      case BinaryOperatorType::MINUS:
        Emit(
            "sub t0, s0, s1",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
        break;
      case BinaryOperatorType::MUL:
        Emit(
            "mul t0, s0, s1",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
        break;
      case BinaryOperatorType::DIV:
        Emit(
            "sdiv t0, s0, s1",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
        break;
      case BinaryOperatorType::MOD: {
        Temporary temp;
        Emit(
            "sdiv t0, s0, s1",
            {temp},
            {lhs_temp, rhs_temp}
        );
        Emit(
            "mul t0, s0",
            {temp},
            {rhs_temp}
        );
        Emit(
            "sub t0, s0, s1",
            {result_temp},
            {lhs_temp, temp}
        );
        break;
      }
      case BinaryOperatorType::AND:
        Emit(
            "and t0, s0, s1",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
        break;
      case BinaryOperatorType::OR:
        Emit(
            "orr t0, s0, s1",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
        break;
    }
  }
  tos_value_ = result_temp;
}

void CodeGenerationVisitor::Visit(TempExpression* temp_expression) {
  tos_value_ = temp_expression->temporary_;
}

void CodeGenerationVisitor::Visit(MemExpression* mem_expression) {
  Temporary result_temp;
  if (mem_expression->expression_->IsBinopExpression()) {
    // MEM(BINOP(OP, e1, e2))
    
    auto binop = dynamic_cast<BinopExpression*>(mem_expression->expression_);
    
    if (binop->operator_type_ == BinaryOperatorType::PLUS) {
      // MEM(BINOP(PLUS, e1, e2))
      
      if (binop->first_->IsConstExpression()) {
        // MEM(BINOP(PLUS, CONST(i), e1))
        
        auto const_lhs = dynamic_cast<ConstExpression*>(binop->first_);
        Temporary rhs_temp = Accept(binop->second_);
        
        Emit(
            "ldr t0, [s0, #" + std::to_string(const_lhs->Value()) + "]",
            {result_temp},
            {rhs_temp}
        );
      } else if (binop->second_->IsConstExpression()) {
        // MEM(BINOP(PLUS, e1, CONST(i)))
        
        Temporary lhs_temp = Accept(binop->first_);
        auto const_rhs = dynamic_cast<ConstExpression*>(binop->second_);
        
        Emit(
            "ldr t0, [s0, #" + std::to_string(const_rhs->Value()) + "]",
            {result_temp},
            {lhs_temp}
        );
      } else {
        // MEM(BINOP(PLUS, e1, e2)) - trivial case
        
        Temporary lhs_temp = Accept(binop->first_);
        Temporary rhs_temp = Accept(binop->second_);
        
        Emit(
            "ldr t0, [s0, s1]",
            {result_temp},
            {lhs_temp, rhs_temp}
        );
      }
    } else {
      // MEM(BINOP) - trivial case
      
      Temporary binop_temp = Accept(mem_expression->expression_);
      
      Emit(
          "ldr t0, [s0]",
          {result_temp},
          {binop_temp}
      );
    }
  } else {
    // MEM(e1) - trivial case
    
    Temporary expression_temp = Accept(mem_expression->expression_);
    
    Emit(
        "ldr t0, [s0]",
        {result_temp},
        {expression_temp}
    );
  }
  tos_value_ = result_temp;
}

void CodeGenerationVisitor::Visit(JumpStatement* jump_statement) {
  Emit("b " + jump_statement->label_.ToString(), {}, {});
}

void CodeGenerationVisitor::Visit(CallExpression* call_expression) {
  Temporary result_temp("r0");
  EmitCallExpression(result_temp, call_expression);
  tos_value_ = result_temp;
}

void CodeGenerationVisitor::Visit(ExpressionList* expression_list) {
  // Processed in parent (CallExpression)
}

void CodeGenerationVisitor::Visit(NameExpression* name_expression) {
  // Processed in parent (CallExpression)
}

void CodeGenerationVisitor::Visit(EseqExpression* eseq_expression) {
  eseq_expression->statement_->Accept(this);
  if (eseq_expression->expression_->IsTempExpression()) {
    // ESEQ(s1, TEMP)
    
    auto temp_expression = dynamic_cast<TempExpression*>(eseq_expression->expression_);
    tos_value_ = temp_expression->temporary_;
  } else {
    // ESEQ(s1, e1) - trivial case
    
    tos_value_ = Accept(eseq_expression->expression_);
  }
}

std::vector<Instruction> CodeGenerationVisitor::GetInstructions() const {
  std::vector<Instruction> instructions;
  instructions.push_back(instructions_[0]); // LabelStatement
  
  // Save old frame pointer on stack
  instructions.push_back(
      Instruction(
          "str s0, [sp, #-4]!",
          {},
          {Temporary("fp")}
      )
  );
  
  // Save old stack pointer in frame pointer
  instructions.push_back(
      Instruction(
          "add t0, sp, #0",
          {Temporary("fp")},
          {}
      )
  );
  
  // Allocate space for arguments and local variables on stack
  instructions.push_back(
      Instruction(
          "sub sp, sp, #" + std::to_string(frame_size_),
          {},
          {}
      )
  );
  
  // Save arguments on stack
  for (int index = 0; index < std::min(static_cast<int>(num_arguments_), 4); ++index) {
    instructions.push_back(
        Instruction(
            "str s0, [s1, #" + std::to_string(4 * -(index + 1)) + "]",
            {},
            {Temporary("r" + std::to_string(index)), Temporary("fp")}
        )
    );
  }
  
  // Save callee-save registers
  instructions.push_back(Instruction("push {r4-r10, lr}", {}, {}));
  
  for (size_t index = 1; index < instructions_.size(); ++index) {
    instructions.push_back(instructions_[index]);
  }
  
  // Restore callee-save registers
  instructions.push_back(
      Instruction(
          "pop {r4-r10, lr}",
          {},
          {}
      )
  );
  
  // Move return value to r0
  instructions.push_back(
      Instruction(
          "mov t0, s0",
          {Temporary("r0")},
          {Temporary("::return_value")}
      )
  );
  
  // Restore old stack pointer
  instructions.push_back(
      Instruction(
          "add sp, s0, #0",
          {},
          {Temporary("fp")}
      )
  );
  
  // Restore old frame pointer
  instructions.push_back(
      Instruction(
          "ldr t0, [sp], #4",
          {Temporary("fp")},
          {}
      )
  );
  
  // Return to caller
  instructions.push_back(Instruction("bx lr", {}, {}));
  
  return instructions;
}

void CodeGenerationVisitor::Emit(std::string str,
                                 std::vector<Temporary> targets,
                                 std::vector<Temporary> sources) {
  instructions_.emplace_back(std::move(str), std::move(targets), std::move(sources));
}

void CodeGenerationVisitor::PrintInstructions(const std::string& filename) const {
  std::ofstream stream(filename);
  for (const auto& instruction: GetInstructions()) {
    if (instruction.GetStr().back() != ':') {
      stream << "    ";
    }
    instruction.Print(stream);
  }
}

void CodeGenerationVisitor::EmitCallExpression(const Temporary& result_temp, CallExpression* call_expression) {
  ExpressionList* expression_list = call_expression->args_;
  auto function_name = dynamic_cast<NameExpression*>(call_expression->function_name_);
  
  std::vector<Temporary> args_temps;
  for (auto expression : expression_list->expressions_) {
    args_temps.push_back(Accept(expression));
  }
  
  if (function_name->label_.ToString() == "printf") {
    Emit("ldr t0, =printf_fmt", {Temporary("r0")}, {});
    
    Temporary arg_temp = args_temps[0];
    Emit("mov t0, s0", {Temporary("r1")}, {arg_temp});
    
    Emit("bl printf", {}, {});
    return;
  } else if (function_name->label_.ToString() == "assert") {
    Temporary arg_temp = args_temps[0];
    Emit("cmp s0, #0", {}, {arg_temp});
    Emit("swieq 0x11", {}, {});
    return;
  }
  
  for (size_t index = 0; index < std::min(static_cast<int>(args_temps.size()), 4); ++index) {
    Emit(
        "mov t0, s0",
        {Temporary("r" + std::to_string(index))},
        {args_temps[index]}
    );
  }
  
  for (size_t index = 4; index < args_temps.size(); ++index) {
    Emit(
        "str s0, [sp, #" + std::to_string(4 * -(index + 2)),
        {},
        {args_temps[index]}
    );
  }
  
  Emit(
      "bl " + function_name->label_.ToString(),
      {},
      {}
  );
  
  Emit(
      "mov t0, s0",
      {result_temp},
      {Temporary("r0")}
  );
}

}