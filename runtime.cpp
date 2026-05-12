#include "runtime.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Environment::Environment(std::shared_ptr<Environment> parent_env) : parent(std::move(parent_env)) {}

void Environment::define(const std::string& name, const ValuePtr& value) {
    values[name] = value;
}

ValuePtr Environment::lookup(const std::string& name) const {
    const auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }
    if (parent) {
        return parent->lookup(name);
    }
    throw std::runtime_error("undefined symbol: " + name);
}

ValuePtr make_nil() {
    auto v = std::make_shared<Value>();
    v->data = std::monostate{};
    return v;
}

ValuePtr make_number(double number) {
    auto v = std::make_shared<Value>();
    v->data = number;
    return v;
}

ValuePtr make_bool(bool value) {
    auto v = std::make_shared<Value>();
    v->data = value;
    return v;
}

ValuePtr make_list(const std::vector<ValuePtr>& elements) {
    auto list = std::make_shared<ListValue>();
    list->elements = elements;

    auto v = std::make_shared<Value>();
    v->data = list;
    return v;
}

ValuePtr make_lambda(const std::vector<std::string>& params, const Expr& body, const std::shared_ptr<Environment>& closure) {
    auto lambda = std::make_shared<LambdaFunction>();
    lambda->params = params;
    lambda->body = body;
    lambda->closure = closure;

    auto v = std::make_shared<Value>();
    v->data = lambda;
    return v;
}

ValuePtr make_builtin(const Value::BuiltinFunction& fn) {
    auto v = std::make_shared<Value>();
    v->data = fn;
    return v;
}

bool is_list_value(const ValuePtr& value) {
    return std::holds_alternative<std::shared_ptr<ListValue>>(value->data);
}

std::shared_ptr<ListValue> as_list(const ValuePtr& value) {
    if (!is_list_value(value)) {
        throw std::runtime_error("expected list value");
    }
    return std::get<std::shared_ptr<ListValue>>(value->data);
}

double as_number(const ValuePtr& value) {
    if (!std::holds_alternative<double>(value->data)) {
        throw std::runtime_error("expected numeric value");
    }
    return std::get<double>(value->data);
}

bool is_truthy(const ValuePtr& value) {
    if (std::holds_alternative<std::monostate>(value->data)) {
        return false;
    }
    if (std::holds_alternative<bool>(value->data)) {
        return std::get<bool>(value->data);
    }
    if (std::holds_alternative<std::shared_ptr<ListValue>>(value->data)) {
        return !std::get<std::shared_ptr<ListValue>>(value->data)->elements.empty();
    }
    return true;
}

std::string value_to_string(const ValuePtr& value) {
    if (std::holds_alternative<std::monostate>(value->data)) {
        return "nil";
    }
    if (std::holds_alternative<double>(value->data)) {
        std::ostringstream out;
        out << std::get<double>(value->data);
        return out.str();
    }
    if (std::holds_alternative<bool>(value->data)) {
        return std::get<bool>(value->data) ? "true" : "false";
    }
    if (std::holds_alternative<std::shared_ptr<ListValue>>(value->data)) {
        const auto& elements = std::get<std::shared_ptr<ListValue>>(value->data)->elements;
        std::ostringstream out;
        out << "(";
        for (size_t i = 0; i < elements.size(); i++) {
            if (i > 0) {
                out << " ";
            }
            out << value_to_string(elements[i]);
        }
        out << ")";
        return out.str();
    }
    if (std::holds_alternative<std::shared_ptr<LambdaFunction>>(value->data)) {
        return "<lambda>";
    }
    return "<builtin>";
}
