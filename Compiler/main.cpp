#include <iostream>
#include <driver.hh>

int main(int argc, char** argv) {
  Driver driver;

  for (int i = 1; i < argc; ++i) {
    if (argv[i] == std::string("-p")) {
      driver.trace_parsing_ = true;
    } else if (argv[i] == std::string("-s")) {
      driver.trace_scanning_ = true;
    } else if (argv[i] == std::string("--src")) {
      int result = driver.Parse(argv[i + 1]);

      if (result == 0) {
        std::cout << "Parsed successfully" << std::endl;
      } else {
        std::cout << "Error while parsing" << std::endl;
        exit(1);
      }

      driver.PrintTree("PrintVisitor_output");
      std::cout << "Saved PrintVisitor output to \"PrintVisitor_output\"" << std::endl;

      driver.Evaluate("SymbolTreeVisitor_output");
      std::cout << "Saved SymbolTreeVisitor output to \"SymbolTreeVisitor_output\"" << std::endl;

//            driver.RunInterpreter("Interpreter_output");
//            std::cout << "Saved Interpreter output to \"Interpreter_output\"" << std::endl;
      break;
    } else {
      exit(1);
    }
  }

  return 0;
}