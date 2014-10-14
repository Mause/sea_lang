cmake_minimum_required(VERSION 2.8)
project(sea_lang)

set(CMAKE_C_FLAGS "-g -Werror ${CMAKE_C_FLAGS}")

find_package(BISON)
find_package(FLEX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/parser)
bison_target(SeaParser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammar.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser/grammar.c
)
flex_target(SeaLexer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/flex_parser.l
    ${CMAKE_CURRENT_BINARY_DIR}/parser/flex_parser.c
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
    src/compile.c
    src/eval.c
    src/sea.c
    src/stack.c

    src/ast/ast.c
    src/ast/render_ast.c
    src/parser/grammar_handlers.c

    src/object/none.c
    src/object/code_object.c
    src/object/object.c

    src/ast_compiler/ast_compiler.c

    ${BISON_SeaParser_OUTPUTS}
    ${FLEX_SeaLexer_OUTPUTS}
)
set_target_properties(libsea PROPERTIES PREFIX "")
target_link_libraries(libsea libdict)

add_executable(test_dict src/dict/test_dict.c)
target_link_libraries(test_dict libsea)

add_executable(sea src/main.c)
target_link_libraries(sea libsea)

