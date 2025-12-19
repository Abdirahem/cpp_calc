#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include "Parser.h"

class Evaluator {
public:
    double evaluate(const std::string& expression);

private:
    Parser parser;
};

#endif
