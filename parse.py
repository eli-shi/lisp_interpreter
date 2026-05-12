example = "(hello world, i want to test my tokenize method)"


def tokenize(s: str) -> list:
    return s.replace("(", " ( ").replace(")", " ) ").split()


def read_tokens(tokens: list) -> list:
    """Returns a single parsed expression"""
    if len(tokens) == 0:
        return SyntaxError("unexpected EOF")
    token = tokens.pop(0)
    if token == "(":
        sub_exp = []
        while tokens[0] != ")":
            sub_exp.append(read_tokens(tokens.pop(0)))
        tokens.pop(0)
    elif token == ")":
        raise SyntaxError


# print(tokenize(example))
