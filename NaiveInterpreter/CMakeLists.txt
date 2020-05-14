cmake_minimum_required(VERSION 3.10)

project(MiniJavaInterpreter)
set(
    HEADERS
    ${MiniJavaInterpreter_SOURCE_DIR}/driver.hh
    ${MiniJavaInterpreter_SOURCE_DIR}/parser.hh
    )

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)    

BISON_TARGET(
    MyParser
    parser.y
    ${MiniJavaInterpreter_SOURCE_DIR}/parser.cpp
    DEFINES_FILE ${MiniJavaInterpreter_SOURCE_DIR}/parser.hh
    COMPILE_FLAGS --graph
)

FLEX_TARGET(
    MyScanner
    scanner.l
    ${MiniJavaInterpreter_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
    MiniJavaInterpreter
    main.cpp
    driver.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_include_directories(MiniJavaInterpreter PRIVATE ${MiniJavaInterpreter_SOURCE_DIR})



