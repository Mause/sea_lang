from pyparsing import ParseException


class SeaSyntaxError(Exception):
    def __init__(self, pstr, loc, msg, elem):
        self.exc = ParseException(pstr, loc, msg, elem)

    def __str__(self):
        return 'line {}, col {}: {}'.format(
            self.exc.lineno,
            self.exc.column,
            self.exc.msg
        )


def syntax_error(msg):
    def wrapped(instring, loc, toks, err):
        raise SeaSyntaxError(instring, loc, msg, toks) from None

    return wrapped
