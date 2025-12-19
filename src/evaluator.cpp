#include "evaluator.h"

double Evaluator::evaluate(const std::string& expression) {
    size_t pos = 0;
    return parser.parseStatement(expression, pos);
}
