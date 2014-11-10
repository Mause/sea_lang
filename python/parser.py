import logging
from grammar_definitions import program
from grammar_declarations import DEBUG


from pyparsing import And
And.__repr__ = And.__str__


class Parser():
    def __init__(self, filename):
        self.filename = filename

        with open(filename) as fh:
            self.contents = fh.read()

        logging.basicConfig(level=logging.DEBUG)

        program.setDebug(DEBUG)
        program.verbose_stacktrace = True
        program.setFailAction(self.fail)

    def fail(self, *args, **kwargs):
        print('Failed to parse', self.filename)

    def parse(self):
        print(('#' * 20) + '\n' + self.contents + '\n' + ('#' * 20))
        # import pudb
        # pudb.set_trace()
        p, = program.parseString(
            self.contents,
            parseAll=True
        )

        from pprint import pprint
        print('imports:')
        pprint(p.imports)

        print()
        print('functions:')
        pprint(p.functions)


def main():
    Parser('../test.sea').parse()


if __name__ == '__main__':
    main()
