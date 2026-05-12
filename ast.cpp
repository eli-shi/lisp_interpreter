#include "ast.hpp"

void print_expr(const Expr& expr, std::ostream& out) {
    if (expr.is_atom()) {
        const Atom& atom = expr.atom();
        if (atom.is_symbol()) {
            out << atom.symbol();
        } else if (atom.is_number()) {
            out << atom.number();
        } else {
            out << (atom.boolean() ? "true" : "false");
        }
        return;
    }

    out << "(";
    const auto& list = expr.list();
    for (size_t i = 0; i < list.size(); i++) {
        if (i > 0) {
            out << " ";
        }
        print_expr(list[i], out);
    }
    out << ")";
}
