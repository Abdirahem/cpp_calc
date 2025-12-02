#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include "Parser.h"

class Calculator {
public:
    double evaluate(const std::string& expression);

private:
    Parser parser;
};

#endif
