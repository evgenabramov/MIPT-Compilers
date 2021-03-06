#include "driver.hh"

#include "PrintVisitor.hpp"
#include "SymbolTreeVisitor.hpp"
#include "MethodCallVisitor.h"
#include "IRTreeBuildVisitor.h"

#include <IRTree/Visitors/PrintVisitor.h>
#include <IRTree/Visitors/DoubleCallEliminateVisitor.h>
#include <IRTree/Visitors/LinearizationVisitor.h>
#include <IRTree/Visitors/ESEQEliminateVisitor.h>
#include <IRTree/Visitors/BlockBorderVisitor.h>
#include <IRTree/Visitors/BlockBuildVisitor.h>
#include <IRTree/Blocks/BlockGraph.hpp>
#include <IRTree/Visitors/CodeGenerationVisitor.h>
#include <IRTree/Instructions/ControlFlowGraph.hpp>
#include <IRTree/Instructions/InterferenceGraph.hpp>

#include "parser.hh"

#include <fstream>

Driver::Driver() :
    trace_parsing_(false),
    trace_scanning_(false),
    scanner_(*this), parser_(scanner_, *this) {}

int Driver::Parse(const std::string& filename) {
  filename_ = filename;
  location_.initialize(&filename_);
  BeginScan();
  parser_.set_debug_level(trace_parsing_);
  int res = parser_();
  EndScan();
  return res;
}

void Driver::BeginScan() {
  scanner_.set_debug(trace_scanning_);
  
  if (filename_.empty() || filename_ == "-") {
    std::cout << "Empty filename" << std::endl;
  } else {
    stream_.open(filename_);
    std::cout << filename_ << std::endl;
    scanner_.yyrestart(&stream_);
  }
}

void Driver::EndScan() {
  stream_.close();
}

void Driver::PrintTree(const std::string& filename) {
  ast::PrintVisitor visitor(filename);
  visitor.Visit(program_);
}

//void Driver::RunInterpreter(const std::string& filename) {
//    ast::Interpreter interpreter(filename);
//    interpreter.Visit(program_);
//    interpreter.PrintReturnValue();
//}

void Driver::Evaluate(const std::string& filename) {
  using namespace ast;
  
  SymbolTreeVisitor symbol_tree_visitor(filename);
  symbol_tree_visitor.Visit(program_);
  
  std::cout << "SymbolTreeVisitor finished work" << std::endl
            << "Tree layers (tab = depth in tree, BFS):" << std::endl;
  // Layers info:
  symbol_tree_visitor.OutputTreeLayers();
  
  // Classes info:
  symbol_tree_visitor.OutputClassesInfo();
  
  // For correct method_call_visitor work
  symbol_tree_visitor.FillClassStorage();
  
  std::shared_ptr<MethodType> main_method = std::dynamic_pointer_cast<MethodType>(
      symbol_tree_visitor.GetScopeLayerTree()->GetRoot()->Get("main"));
  
  MethodCallVisitor method_call_visitor(
      symbol_tree_visitor.GetScopeLayerTree()->GetScopeLayer("main"), main_method,
      new SimpleObject(new PrimitiveSimpleType(new SimpleType("int")), 0));
  
  method_call_visitor.SetTree(symbol_tree_visitor.GetScopeLayerTree());
  
  method_call_visitor.Visit(main_method->GetMethodDeclaration());
  
  IRTreeBuildVisitor irtree_build_visitor(symbol_tree_visitor.GetScopeLayerTree());
  irtree_build_visitor.Visit(program_);
  
  IrtMapping methods = irtree_build_visitor.GetMethodTrees();
  
  std::vector<irt::Instruction> program_instructions;
  program_instructions.push_back(irt::Instruction(".text", {}, {}));
  program_instructions.push_back(irt::Instruction(".global main", {}, {}));
  program_instructions.push_back(irt::Instruction("", {}, {}));
  
  for (auto&[method_name, method_info] : methods) {
    auto&[method_frame, method_statement] = method_info;
    
    irt::PrintVisitor print_visitor(method_name, "_IRTree_raw");
    method_statement->Accept(&print_visitor);
    
    irt::DoubleCallEliminateVisitor call_eliminate_visitor;
    method_statement->Accept(&call_eliminate_visitor);
    method_statement = call_eliminate_visitor.GetTree();
    
    print_visitor.ChangeStream(method_name, "_IRTree_without_double_call");
    method_statement->Accept(&print_visitor);
    
    irt::ESEQEliminateVisitor eseq_eliminate_visitor;
    method_statement->Accept(&eseq_eliminate_visitor);
    method_statement = eseq_eliminate_visitor.GetTree();
    
    print_visitor.ChangeStream(method_name, "_IRTree_without_ESEQ");
    method_statement->Accept(&print_visitor);
    
    irt::LinearizationVisitor linearization_visitor;
    method_statement->Accept(&linearization_visitor);
    method_statement = linearization_visitor.GetTree();
    
    print_visitor.ChangeStream(method_name, "_IRTree_linearized");
    method_statement->Accept(&print_visitor);
    
    irt::BlockBorderVisitor block_border_visitor(method_name);
    method_statement->Accept(&block_border_visitor);
    method_statement = block_border_visitor.GetTree();
    
    print_visitor.ChangeStream(method_name, "_IRTree_with_blocks");
    method_statement->Accept(&print_visitor);
    
    irt::BlockBuildVisitor block_build_visitor;
    method_statement->Accept(&block_build_visitor);
    
    irt::BlockGraph block_graph = block_build_visitor.GetBlockGraph();
    block_graph.OutputGraph(method_name, "_IRTree_blocks");
    block_graph.OutputTraces(method_name, "_IRTree_traces");
    
    irt::CodeGenerationVisitor code_generation_visitor(
        method_frame->GetNumArguments(),
        method_frame->GetFrameSize()
    );
    method_statement->Accept(&code_generation_visitor);
    code_generation_visitor.PrintInstructions(method_name + ".s");
    
    size_t frame_size = method_frame->GetFrameSize();
    std::vector<irt::Instruction> instructions = code_generation_visitor.GetInstructions();
    
    size_t iteration = 0;
    bool should_retry_pipeline;
    
    do {
      ++iteration;
      
      irt::ControlFlowGraph control_flow_graph(frame_size, instructions);
      irt::InterferenceGraph interference_graph = control_flow_graph.BuildInterferenceGraph();
      
      control_flow_graph.OutputGraph(method_name + "_ControlFlowGraph" + std::to_string(iteration));
      
      interference_graph.Output(method_name + "_InterferenceGraph" + std::to_string(iteration));
      
      should_retry_pipeline = interference_graph.ColorNodes();
      
      frame_size = interference_graph.GetFrameSize();
      instructions = interference_graph.GetInstructions();
      
      // For functions separation
      instructions.push_back(irt::Instruction("", {}, {}));
      
      irt::PrintInstructions(method_name + std::to_string(iteration) + ".s", instructions);
      
    } while (should_retry_pipeline);
    
    program_instructions.insert(program_instructions.end(), instructions.begin(), instructions.end());
  }
  
  program_instructions.push_back(irt::Instruction(".data", {}, {}));
  program_instructions.push_back(irt::Instruction("printf_fmt:", {}, {}));
  program_instructions.push_back(irt::Instruction(R"(.string "%d\n")", {}, {}));
  
  irt::PrintInstructions("final_program.s", program_instructions);
}