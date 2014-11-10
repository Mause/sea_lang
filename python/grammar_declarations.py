from helpers import (
    create_forloop,
    create_import,
    create_function_call,
    create_declaration,
    create_assignment,
    create_function,
    SForward,
    listify
)
# from exceptions import syntax_error
from nodes import Program, FunctionHeader
DEBUG = True

argument_declarations = SForward('argument_declarations')
expression = SForward('expression')
function_call = SForward('function call')
body = SForward('body')
forloop = SForward(
    'forloop', lambda s, loc, toks: create_forloop(toks[1], toks[3], toks[5])
)
whileloop = SForward(
    'whileloop', lambda: Exception('World')
)
assignment = SForward(
    'assignment', lambda s, loc, toks: create_assignment(toks[0], toks[2])
)
declaration = SForward(
    'declaration', lambda s, loc, toks: create_declaration(toks[1])
)
curly_scope = SForward(
    'curly_scope', lambda s, loc, toks: toks[1]
)
function_call = SForward(
    'function_call',
    lambda s, loc, toks: create_function_call(toks[0], toks[2])
)
function = SForward(
    'function',
    lambda s, loc, toks: create_function(toks[0].name, toks[0].args, toks[1:])
)
simport = SForward(
    'import', lambda s, loc, toks: create_import(toks[1])
)

function_header = SForward(
    'function_header',
    lambda s, loc, toks: FunctionHeader(toks[1], toks[3:-1])
)
arguments = SForward('arguments')
statement = SForward('statement')
# .setFailAction(
#     syntax_error('Invalid statement')
# )
statements = SForward('statements', listify)
block_statement = SForward('block_statement')
normal_statement = SForward('normal_statement')
string = SForward('string')
comment = SForward('comment')
imports = SForward('imports', listify)
functions = SForward('functions', listify)
program = (
    SForward('program', lambda s, loc, toks: Program(toks[0], toks[1]))
    .ignore(comment)
)

# forloop.setFailAction(lambda loc: syntax_error(loc, 'Invalid for loop'))
