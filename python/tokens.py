# %x COMMENT
from pyparsing import Literal, nums, Regex, Word, Keyword

NUMBER = Word(nums).addParseAction(lambda s, loc, toks: int(toks[0]))
OPEN_CURLY = Literal('{')
CLOSE_CURLY = Literal('}')
SEMICOLON = Literal(';')
# \n                      { yylloc.last_line++; };
# [ \t]+                  /* ignore whitespace */;

CLOSE_BRACE = Literal(')')
OPEN_BRACE = Literal('(')
VAR = Keyword('var').setName('var')
FUNC = Keyword('func').setName('func')
IMPORT = Keyword('import').setName('import')
IN = Keyword('in').setName('in')
FOR = Keyword('for').setName('for')
WHILE = Keyword('while').setName('while')

COMMA = Literal(',')
EQUALS = Literal('=')
# <INITIAL>#              BEGIN(COMMENT);
# <COMMENT>"\n"           BEGIN(INITIAL);
# <COMMENT>.              {};
QUOTE = Literal('"')

IDENTIFIER = (
    Regex(r'([a-zA-Z_][_a-zA-Z0-9]*)').setName('IDENTIFIER')
    .setDebug(False)
)
