from pyparsing import Forward
from nodes import (
    Function, Assignment, FunctionCall, Import, Declaration,
    Number, Identifier, Forloop
)


def create_function(name, args, body):
    return Function(name, args, body)


def create_assignment(ident, expr):
    return Assignment(ident, expr)


def create_function_call(function, arguments):
    return FunctionCall(function, arguments.nodes)


def create_import(ident):
    return Import(ident)


def create_declaration(obj):
    if isinstance(obj, Assignment):
        return Declaration(obj.ident, obj.expr)

    return Declaration(obj, None)


def create_number(string):
    return Number(string)


def create_identifier(string):
    return Identifier(string)


def append_to_manynodes(manynodes, new_node):
    return manynodes + [new_node]


def create_forloop(ident, iterable, body):
    return Forloop(ident, iterable, body)


def raise_exception(exc):
    raise exc


listify = lambda s, loc, toks: toks.asList()


S = type('S', (str,), {'strip': lambda self, x="": self})


class SForward(Forward):
    def __init__(self, name, parse_action=None):
        super().__init__()

        self.setDebug(True)

        self.setName(name)
        if parse_action:
            parse_action.__qualname__ = '{}_parse_action'.format(name)
            self.addParseAction(parse_action)

    def __repr__(self):
        return self.__str__()


# def create_error(char* yytext, struct YYLTYPE lloc, char* message):
#     err = Error()

#     err.err = calloc(1, sizeof(*err.err))
#     err.err.error_location = strdup(yytext)
#     err.err.lloc = lloc
#     err.err.message = message

#     return err
# }
