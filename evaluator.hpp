#pragma once

#include <memory>

#include "ast.hpp"
#include "runtime.hpp"

ValuePtr eval(const Expr& expr, const std::shared_ptr<Environment>& env);
