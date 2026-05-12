#include "evaluator.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::vector<ValuePtr> eval_args(const ExprList& exprs, size_t start_index, const std::shared_ptr<Environment>& env) {
    std::vector<ValuePtr> args;
    for (size_t i = start_index; i < exprs.size(); i++) {
        args.push_back(eval(exprs[i], env));
    }
    return args;
}

std::vector<std::string> read_lambda_params(const Expr& param_expr) {
    if (!param_expr.is_list()) {
        throw std::runtime_error("lambda parameter list must be a list");
    }

    std::vector<std::string> params;
    for (const auto& param : param_expr.list()) {
        if (!param.is_atom() || !param.atom().is_symbol()) {
            throw std::runtime_error("lambda parameters must be symbols");
        }
        params.push_back(param.atom().symbol());
    }
    return params;
}

ValuePtr apply_callable(const ValuePtr& callable, const std::vector<ValuePtr>& args) {
    if (std::holds_alternative<Value::BuiltinFunction>(callable->data)) {
        const auto& builtin = std::get<Value::BuiltinFunction>(callable->data);
        return builtin(args);
    }

    if (std::holds_alternative<std::shared_ptr<LambdaFunction>>(callable->data)) {
        const auto& lambda = std::get<std::shared_ptr<LambdaFunction>>(callable->data);
        if (args.size() != lambda->params.size()) {
            throw std::runtime_error("lambda called with incorrect number of arguments");
        }

        auto call_env = std::make_shared<Environment>(lambda->closure);
        for (size_t i = 0; i < args.size(); i++) {
            call_env->define(lambda->params[i], args[i]);
        }
        return eval(lambda->body, call_env);
    }

    throw std::runtime_error("first element is not a callable value");
}

}  // namespace

ValuePtr eval(const Expr& expr, const std::shared_ptr<Environment>& env) {
    if (expr.is_atom()) {
        const Atom& atom = expr.atom();
        if (atom.is_number()) {
            return make_number(atom.number());
        }
        if (atom.is_bool()) {
            return make_bool(atom.boolean());
        }
        return env->lookup(atom.symbol());
    }

    const ExprList& list = expr.list();
    if (list.empty()) {
        return make_list({});
    }

    if (list[0].is_atom() && list[0].atom().is_symbol()) {
        const std::string& op = list[0].atom().symbol();

        if (op == "define") {
            if (list.size() != 3) {
                throw std::runtime_error("define expects exactly 2 arguments");
            }
            if (!list[1].is_atom() || !list[1].atom().is_symbol()) {
                throw std::runtime_error("define target must be a symbol");
            }

            const std::string& name = list[1].atom().symbol();
            ValuePtr value = eval(list[2], env);
            env->define(name, value);
            return value;
        }

        if (op == "if") {
            if (list.size() != 4) {
                throw std::runtime_error("if expects exactly 3 arguments");
            }

            ValuePtr condition = eval(list[1], env);
            if (is_truthy(condition)) {
                return eval(list[2], env);
            }
            return eval(list[3], env);
        }

        if (op == "lambda") {
            if (list.size() != 3) {
                throw std::runtime_error("lambda expects exactly 2 arguments");
            }
            std::vector<std::string> params = read_lambda_params(list[1]);
            return make_lambda(params, list[2], env);
        }
    }

    ValuePtr callable = eval(list[0], env);
    std::vector<ValuePtr> args = eval_args(list, 1, env);
    return apply_callable(callable, args);
}
