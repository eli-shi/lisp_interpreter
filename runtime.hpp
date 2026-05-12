#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "ast.hpp"

struct Value;
struct Environment;
struct LambdaFunction;
struct ListValue;

using ValuePtr = std::shared_ptr<Value>;

struct ListValue {
    std::vector<ValuePtr> elements;
};

struct LambdaFunction {
    std::vector<std::string> params;
    Expr body;
    std::shared_ptr<Environment> closure;
};

struct Value {
    using BuiltinFunction = std::function<ValuePtr(const std::vector<ValuePtr>&)>;

    std::variant<std::monostate, double, bool, std::shared_ptr<ListValue>, std::shared_ptr<LambdaFunction>, BuiltinFunction> data;
};

struct Environment {
    explicit Environment(std::shared_ptr<Environment> parent_env = nullptr);

    void define(const std::string& name, const ValuePtr& value);
    ValuePtr lookup(const std::string& name) const;

    std::unordered_map<std::string, ValuePtr> values;
    std::shared_ptr<Environment> parent;
};

ValuePtr make_nil();
ValuePtr make_number(double number);
ValuePtr make_bool(bool value);
ValuePtr make_list(const std::vector<ValuePtr>& elements);
ValuePtr make_lambda(const std::vector<std::string>& params, const Expr& body, const std::shared_ptr<Environment>& closure);
ValuePtr make_builtin(const Value::BuiltinFunction& fn);

bool is_list_value(const ValuePtr& value);
std::shared_ptr<ListValue> as_list(const ValuePtr& value);
double as_number(const ValuePtr& value);
bool is_truthy(const ValuePtr& value);
std::string value_to_string(const ValuePtr& value);
