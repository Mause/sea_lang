import ast


class NodeMeta(type):
    def __init__(cls, name, bases, dct):
        if '_fields' in dct and cls.__init__ is object.__init__:
            # build the custom __init__
            init = NodeMeta.build_init(cls)

            # compile the init function
            ctx = {}
            eval(compile(init, '<string>', 'exec'), ctx)

            # attach the built function
            cls.__init__ = ctx['__init__']

        return super().__init__(name, bases, dct)

    def build_init(cls):
        # build arguments objects
        args = [ast.arg('self', None)] + [
            ast.arg(arg, None)
            for arg in cls._fields
        ]

        body = [
            ast.Assign(
                targets=[
                    ast.Attribute(
                        value=ast.Name(id='self', ctx=ast.Load()),
                        attr=arg, ctx=ast.Store()
                    )
                ],
                value=ast.Name(id=arg, ctx=ast.Load())
            )
            for arg in cls._fields
        ]

        function = ast.FunctionDef(
            '__init__',
            ast.arguments(args, None, [], [], None, []),
            body, [], None
        )

        return ast.fix_missing_locations(
            ast.Module(body=[function])
        )


class Node(metaclass=NodeMeta):
    def __str__(self):
        return '<{} {}>'.format(
            self.__class__.__name__,
            ', '.join(
                '{}={}'.format(
                    name,
                    (
                        '"{}"'
                        if isinstance(getattr(self, name), str)
                        else '{}'
                    ).format(getattr(self, name))
                )
                for name in self.__class__._fields
            )
        )

    __repr__ = __str__


class Forloop(Node):
    _fields = ['iter_ident', 'iterable', 'body']


Forloop('name', object(), [])


class Assignment(Node):
    _fields = ['ident', 'expr']


class Declaration(Node):
    _fields = ['ident', 'expr']


class Many_nodes(Node):
    _fields = ['nodes']


class FunctionCall(Node):
    _fields = ['function', 'arguments']


class Function(Node):
    _fields = ['name', 'args', 'body']


class FunctionHeader(Node):
    _fields = ['name', 'args']


class Identifier(Node):
    _fields = ['ident']


class Import(Identifier):
    pass


class Number(Node):
    pass


class Program(Node):
    _fields = ['imports', 'functions']

# class Error(Node):
#     char* error_location
#     YYLTYPE lloc
#     char* message
