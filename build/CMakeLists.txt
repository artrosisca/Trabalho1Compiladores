cmake_minimum_required(VERSION 3.11)

project(trab)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

set(SRC_DIR "../src")
set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

find_package(FLEX)
find_package(BISON)

set(LEXER_OUT "${PARSER_DIR}/flex.c")
set(PARSER_OUT "${PARSER_DIR}/bison.tab.c")

flex_target(LEXER "${SRC_DIR}/flex.l" "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/flex.h")
bison_target(PARSER "${SRC_DIR}/bison.y" "${PARSER_OUT}" DEFINES_FILE "${PARSER_DIR}/bison.tab.h")
add_flex_bison_dependency(LEXER PARSER)

#add_compile_options(-O2 -fsanitize=address -fno-omit-frame-pointer)
#add_link_options(-fsanitize=address)

add_executable(trab "${LEXER_OUT}" "${PARSER_OUT}" "${SRC_DIR}/arvore.c")
