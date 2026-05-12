#include "parser.hpp"

#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

std::vector<std::string> tokenize(const std::string& source) {
    std::string padded;
    for (char c : source) {
        if (c == '(') {
            padded += " ( ";
        } else if (c == ')') {
            padded += " ) ";
        } else {
            padded += c;
        }
    }

    std::vector<std::string> tokens;
    std::istringstream stream(padded);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool is_number_token(const std::string& token, double& value_out) {
    char* end = nullptr;
    const char* start = token.c_str();
    const double parsed = std::strtod(start, &end);
    if (start == end || *end != '\0') {
        return false;
    }
    value_out = parsed;
    return true;
}

Atom parse_atom(const std::string& token) {
    if (token == "true") {
        return Atom{true};
    }
    if (token == "false") {
        return Atom{false};
    }

    double number_value = 0.0;
    if (is_number_token(token, number_value)) {
        return Atom{number_value};
    }

    return Atom{token};
}

class Parser {
public:
    explicit Parser(std::vector<std::string> tokens) : tokens_(std::move(tokens)) {}

    Expr parse_program() {
        Expr expr = parse_expr();
        if (has_more()) {
            throw std::runtime_error("unexpected trailing tokens after complete expression");
        }
        return expr;
    }

private:
    std::vector<std::string> tokens_;
    size_t pos_ = 0;

    bool has_more() const { return pos_ < tokens_.size(); }

    const std::string& peek() const {
        if (!has_more()) {
            throw std::runtime_error("unexpected EOF");
        }
        return tokens_[pos_];
    }

    std::string pop() {
        const std::string token = peek();
        pos_++;
        return token;
    }

    Expr parse_expr() {
        const std::string token = pop();

        if (token == "(") {
            return parse_list();
        }
        if (token == ")") {
            throw std::runtime_error("unexpected )");
        }

        return Expr{parse_atom(token)};
    }

    Expr parse_list() {
        ExprList sub_exp;
        while (true) {
            if (!has_more()) {
                throw std::runtime_error("missing closing )");
            }
            if (peek() == ")") {
                pop();
                break;
            }
            sub_exp.push_back(parse_expr());
        }
        return Expr{sub_exp};
    }
};

}  // namespace

Expr parse(const std::string& source) {
    Parser parser(tokenize(source));
    return parser.parse_program();
}
