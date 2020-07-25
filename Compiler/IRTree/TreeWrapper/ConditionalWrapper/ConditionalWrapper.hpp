#pragma once

#include "../SubtreeWrapper.hpp"
#include "IRTree/Generators/Temporary.h"
#include "../../Nodes/Expression/TempExpression.hpp"
#include "../../Nodes/Expression/ConstExpression.hpp"
#include "../../Nodes/Expression/EseqExpression.hpp"
#include "../../Nodes/Statement/SeqStatement.hpp"
#include "../../Nodes/Statement/MoveStatement.hpp"
#include "../../Nodes/Statement/LabelStatement.hpp"

namespace irt {

class ConditionalWrapper : public SubtreeWrapper {
 public:
  virtual ~ConditionalWrapper() = default;

  Expression* ToExpression() override {
    auto* temp_expression = new TempExpression(Temporary());
    Label label_true;
    Label label_false;
    return new EseqExpression(
        new SeqStatement(
            new MoveStatement(temp_expression, new ConstExpression(1)),
            new SeqStatement(
                ToConditional(label_true, label_false),
                new SeqStatement(
                    new LabelStatement(label_false),
                    new SeqStatement(
                        new MoveStatement(temp_expression, new ConstExpression(0)),
                        new LabelStatement(label_true)
                    )
                )
            )
        ),
        temp_expression
    );
  }

  Statement* ToStatement() override {
    return nullptr;
  }
};

}


