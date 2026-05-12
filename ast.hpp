#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct Atom {
    std::variant<std::string, double, bool> value;

    bool is_symbol() const { return std::holds_alternative<std::string>(value); }
    bool is_number() const { return std::holds_alternative<double>(value); }
    bool is_bool() const { return std::holds_alternative<bool>(value); }

    const std::string& symbol() const { return std::get<std::string>(value); }
    double number() const { return std::get<double>(value); }
    bool boolean() const { return std::get<bool>(value); }
};

struct Expr;
using ExprList = std::vector<Expr>;

struct Expr {
    std::variant<Atom, ExprList> value;

    bool is_atom() const { return std::holds_alternative<Atom>(value); }
    bool is_list() const { return std::holds_alternative<ExprList>(value); }

    const Atom& atom() const { return std::get<Atom>(value); }
    const ExprList& list() const { return std::get<ExprList>(value); }
};

void print_expr(const Expr& expr, std::ostream& out = std::cout);
