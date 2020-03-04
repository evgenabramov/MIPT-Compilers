cmake_minimum_required(VERSION 3.10)

project(ParserExample)
set(
    HEADERS
    ${ParserExample_SOURCE_DIR}/driver.hh
    ${ParserExample_SOURCE_DIR}/parser.hh
    )

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)    

BISON_TARGET(
    MyParser
    parser.y
    ${ParserExample_SOURCE_DIR}/parser.cpp
    DEFINES_FILE ${ParserExample_SOURCE_DIR}/parser.hh
    COMPILE_FLAGS --graph
)

FLEX_TARGET(
    MyScanner
    scanner.l
    ${ParserExample_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
    ParserExample
    main.cpp
    driver.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_include_directories(ParserExample PRIVATE ${ParserExample_SOURCE_DIR})



