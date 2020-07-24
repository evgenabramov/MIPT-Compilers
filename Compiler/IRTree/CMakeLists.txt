add_library(${IRTreeLibrary})

target_include_directories(
        ${IRTreeLibrary}
        PRIVATE

        Generators
        Nodes
        Nodes/Expression
        Nodes/Statement
        TreeWrapper
        TreeWrapper/ConditionalWrapper
        Types
        Visitors
)

target_sources(
        ${IRTreeLibrary}
        PRIVATE

        Generators/Label.cpp
        Generators/Temporary.cpp

        Nodes/BaseElement.hpp
        Nodes/Expression/Expression.hpp
        Nodes/Statement/Statement.hpp
        Nodes/Statement/ExpStatement.hpp
        Nodes/Expression/ConstExpression.hpp
        Nodes/Statement/JumpConditionalStatement.hpp
        Nodes/Statement/MoveStatement.hpp
        Nodes/Statement/SeqStatement.hpp
        Nodes/Statement/LabelStatement.hpp
        Nodes/Expression/BinopExpression.hpp
        Nodes/Expression/TempExpression.hpp
        Nodes/Expression/MemExpression.hpp
        Nodes/Statement/JumpStatement.hpp
        Nodes/Expression/CallExpression.hpp
        Nodes/ExpressionList.hpp
        Nodes/Expression/NameExpression.hpp
        Nodes/Expression/EseqExpression.hpp

        Types/LogicOperatorType.hpp
        Types/BinaryOperatorType.hpp

        TreeWrapper/SubtreeWrapper.hpp
        TreeWrapper/ExpressionWrapper.hpp
        TreeWrapper/StatementWrapper.hpp
        TreeWrapper/ConditionalWrapper/ConditionalWrapper.hpp
        TreeWrapper/ConditionalWrapper/NegateConditionalWrapper.hpp
        TreeWrapper/ConditionalWrapper/AndConditionalWrapper.hpp
        TreeWrapper/ConditionalWrapper/OrConditionalWrapper.hpp
        TreeWrapper/ConditionalWrapper/RelativeConditionalWrapper.hpp

        Visitors/Visitor.h
        Visitors/PrintVisitor.cpp
        Visitors/PrintVisitor.h
        Visitors/BaseElements.h
        Visitors/forward_decl.hpp
        Visitors/DoubleCallEliminateVisitor.cpp
        Visitors/VisitorStruct.h
        Visitors/TemplateVisitor.hpp
        Visitors/ESEQEliminateVisitor.cpp
        Visitors/LinearizationVisitor.cpp
)