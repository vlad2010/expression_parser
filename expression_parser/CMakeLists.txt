cmake_minimum_required(VERSION 2.6.0)

project (expression_parser)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(expression_parser Parser.cpp  Value.cpp  Tokenizer.cpp expression_parser.cpp)

