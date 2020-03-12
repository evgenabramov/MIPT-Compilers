#include <iostream>
#include <driver.hh>

int main(int argc, char** argv) {
    int result = 0;
    Driver driver;

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == std::string("-p")) {
            driver.trace_parsing_ = true;
        } else if (argv[i] == std::string("-s")) {
            driver.trace_scanning_ = true;
        } else if (!driver.Parse(argv[i])) {
            std::cout << driver.result_ << std::endl;
        } else {
            result = 1;
        }
    }

    return result;
}