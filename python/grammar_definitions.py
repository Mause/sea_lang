from exceptions import syntax_error
from tokens import (
    IMPORT, IDENTIFIER, SEMICOLON, FUNC, OPEN_BRACE, OPEN_CURLY, CLOSE_BRACE,
    CLOSE_CURLY, EQUALS, VAR, FOR, IN, NUMBER, QUOTE, WHILE
)
from pyparsing import (
    LineEnd, ZeroOrMore, QuotedString, Optional,
    delimitedList,
)
from grammar_declarations import (
    string, expression, assignment, declaration, curly_scope, block_statement,
    normal_statement, statement, body, whileloop, forloop, arguments,
    function_call, function, function_header, simport, argument_declarations,
    functions, comment, imports, program, statements
)
from helpers import S

string <<= QuotedString(quoteChar=S(QUOTE), endQuoteChar=S(QUOTE))
argument_declarations <<= Optional(delimitedList(IDENTIFIER))
expression <<= (
    IDENTIFIER
    | NUMBER
    | string
    # | function_call
    # | (expression + Literal('+') + expression)
    # | (expression + Literal('-') + expression)
    # | (expression + Literal('*') + expression)
)
assignment <<= IDENTIFIER + EQUALS + expression
declaration <<= VAR - (assignment | IDENTIFIER)
curly_scope <<= OPEN_CURLY + body.copy().setResultsName('body') + CLOSE_CURLY

block_statement <<= forloop | whileloop
normal_statement <<= (
    (declaration + SEMICOLON) |
    (assignment + SEMICOLON) |
    (expression + SEMICOLON)
)
statement <<= block_statement | normal_statement
statements <<= ZeroOrMore(statement)
body <<= statements.copy()
whileloop <<= WHILE + expression + curly_scope
forloop <<= FOR - IDENTIFIER - IN - expression - curly_scope
arguments <<= Optional(delimitedList(expression))
function_call <<= expression + OPEN_BRACE + arguments + CLOSE_BRACE
function_header <<= (
    FUNC - IDENTIFIER.copy().setResultsName('name') -
    (OPEN_BRACE - argument_declarations.setResultsName('args') - CLOSE_BRACE)
)
function <<= function_header + curly_scope
simport <<= IMPORT + (IDENTIFIER.copy().setResultsName('ident')) + SEMICOLON


start = (
    function
    | function_header

    # only this instance of `simport` will throw an error
    | simport.copy().setParseAction(lambda loc: syntax_error(
        loc, 'Out of place import'
    ))
    # | error
)
# import pudb
# old_preParse = start.preParse
# start.callPreparse = True
# start.preParse = lambda instring, loc: (
#     pudb.set_trace() or old_preParse(instring, loc))


functions <<= ZeroOrMore(start)
comment <<= QuotedString(quoteChar="#", endQuoteChar=S(LineEnd()))
imports <<= ZeroOrMore(simport)
program <<= (imports + functions)
