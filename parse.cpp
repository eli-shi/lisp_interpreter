#include <iostream>
#include <memory>
#include <string>

#include "ast.hpp"
#include "builtins.hpp"
#include "evaluator.hpp"
#include "parser.hpp"
#include "runtime.hpp"

void run_parse_case(const std::string& source) {
    std::cout << "Input:  " << source << "\n";
    Expr parsed = parse(source);
    std::cout << "Parsed: ";
    print_expr(parsed);
    std::cout << "\n\n";
}

void run_error_case(const std::string& source) {
    std::cout << "Input:  " << source << "\n";
    try {
        (void)parse(source);
        std::cout << "Error:  expected parser failure but parsing succeeded\n\n";
    } catch (const std::exception& e) {
        std::cout << "Error:  " << e.what() << "\n\n";
    }
}

void run_eval_case(const std::string& source, const std::shared_ptr<Environment>& env) {
    std::cout << "Eval input:  " << source << "\n";
    ValuePtr result = eval(parse(source), env);
    std::cout << "Eval output: " << value_to_string(result) << "\n\n";
}

int main() {
    run_parse_case("(+ 4 5)");
    run_parse_case("(if true 1 0)");
    run_parse_case("(define twice (lambda (a) (+ a a)))");

    run_error_case("(+ 1 2");
    run_error_case("(+ 1 2))");
    run_error_case(")");

    auto env = create_global_env();
    run_eval_case("(+ 4 5)", env);
    run_eval_case("(= 2 2)", env);
    run_eval_case("(= 2 3)", env);
    run_eval_case("(list 1 2 3)", env);
    run_eval_case("(first (list 1 2 3))", env);
    run_eval_case("(rest (list 1 2 3))", env);
    run_eval_case("(append (list 1 2 3) 4)", env);
    run_eval_case("(define twice (lambda (a) (+ a a)))", env);
    run_eval_case("(twice 10)", env);
    run_eval_case("(if (= 1 2) 99 77)", env);
    run_eval_case("(print (twice 7))", env);

    return 0;
}
