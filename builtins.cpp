#include "builtins.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void ensure_arg_count(const std::vector<ValuePtr>& args, size_t expected, const std::string& fn_name) {
    if (args.size() != expected) {
        throw std::runtime_error(fn_name + " expects " + std::to_string(expected) + " arguments");
    }
}

void ensure_min_args(const std::vector<ValuePtr>& args, size_t minimum, const std::string& fn_name) {
    if (args.size() < minimum) {
        throw std::runtime_error(fn_name + " expects at least " + std::to_string(minimum) + " arguments");
    }
}

}  // namespace

std::shared_ptr<Environment> create_global_env() {
    auto env = std::make_shared<Environment>();

    env->define("+", make_builtin([](const std::vector<ValuePtr>& args) {
        ensure_min_args(args, 1, "+");
        double total = 0.0;
        for (const auto& arg : args) {
            total += as_number(arg);
        }
        return make_number(total);
    }));

    env->define("=", make_builtin([](const std::vector<ValuePtr>& args) {
        ensure_arg_count(args, 2, "=");
        const bool equal = as_number(args[0]) == as_number(args[1]);
        if (equal) {
            return make_bool(true);
        }
        return make_list({});
    }));

    env->define("print", make_builtin([](const std::vector<ValuePtr>& args) {
        ensure_arg_count(args, 1, "print");
        std::cout << value_to_string(args[0]) << "\n";
        return args[0];
    }));

    env->define("list", make_builtin([](const std::vector<ValuePtr>& args) {
        return make_list(args);
    }));

    env->define("first", make_builtin([](const std::vector<ValuePtr>& args) {
        ensure_arg_count(args, 1, "first");
        auto list = as_list(args[0]);
        if (list->elements.empty()) {
            throw std::runtime_error("first cannot be used on an empty list");
        }
        return list->elements[0];
    }));

    env->define("rest", make_builtin([](const std::vector<ValuePtr>& args) {
        ensure_arg_count(args, 1, "rest");
        auto list = as_list(args[0]);
        if (list->elements.empty()) {
            return make_list({});
        }

        std::vector<ValuePtr> tail;
        for (size_t i = 1; i < list->elements.size(); i++) {
            tail.push_back(list->elements[i]);
        }
        return make_list(tail);
    }));

    env->define("append", make_builtin([](const std::vector<ValuePtr>& args) {
        ensure_arg_count(args, 2, "append");
        auto list = as_list(args[0]);

        std::vector<ValuePtr> appended = list->elements;
        appended.push_back(args[1]);
        return make_list(appended);
    }));

    return env;
}
