cmake_minimum_required(VERSION 2.8)
project(sea_lang)

set(CMAKE_C_FLAGS "-g -Werror ${CMAKE_C_FLAGS}")

find_package(BISON)
find_package(FLEX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

bison_target(SeaParser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/grammar.y
    ${CMAKE_CURRENT_BINARY_DIR}/grammar.c
)
flex_target(SeaLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flex_parser.l
    ${CMAKE_CURRENT_BINARY_DIR}/flex_parser.c
)

add_flex_bison_dependency(SeaLexer SeaParser)

add_library(libdict
    src/dict/dict.c
    src/dict/dict_entry.c
    src/dict/primes.c
    src/object/none.c
)
set_target_properties(libdict PROPERTIES PREFIX "")

add_library(libsea
    src/ast/ast.c
    src/ast/render_ast.c
    src/compile.c
    src/eval.c
    src/object/none.c
    src/grammar_handlers.c
    src/sea.c
    src/stack.c

    ${BISON_SeaParser_OUTPUTS}
    ${FLEX_SeaLexer_OUTPUTS}
)
set_target_properties(libsea PROPERTIES PREFIX "")

add_executable(test_dict src/dict/test_dict.c)
target_link_libraries(test_dict libsea)

add_executable(main src/main.c)
target_link_libraries(main libsea)

add_executable(stack_machine
    src/stack_machine.c
    src/stack_machine/program.c
    src/stack_machine/instruction.c
)
target_link_libraries(stack_machine libsea)
