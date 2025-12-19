#include "Parser.h"
#include <cctype>
#include <cmath>
#include <stdexcept>

Parser::Parser() {}

// Statement := [Variable '='] Expression
double Parser::parseStatement(const std::string& expr, size_t& pos) {
    while (pos < expr.size() && isspace(expr[pos])) ++pos;

    // Check if the expression starts with a variable assignment
    if (pos < expr.size() && isalpha(expr[pos])) {
        size_t start = pos;
        while (pos < expr.size() && (isalnum(expr[pos]) || expr[pos] == '_')) ++pos;
        std::string varName = expr.substr(start, pos - start);

        // Skip whitespace
        while (pos < expr.size() && isspace(expr[pos])) ++pos;

        if (pos < expr.size() && expr[pos] == '=') {
            ++pos;
            double value = parseExpression(expr, pos);
            variables[varName] = value;
            return value;
        }

        // No '=', roll back — treat as expression with variable
        pos = start;
    }

    return parseExpression(expr, pos);
}

// Expression := Term { ('+' | '-') Term }
double Parser::parseExpression(const std::string& expr, size_t& pos) {
    double value = parseTerm(expr, pos);
    while (true) {
        while (pos < expr.size() && isspace(expr[pos])) ++pos;
        if (pos >= expr.size()) break;
        char op = expr[pos];
        if (op != '+' && op != '-') break;
        ++pos;
        double rhs = parseTerm(expr, pos);
        value = (op == '+') ? value + rhs : value - rhs;
    }
    return value;
}

// Term := Power { ('*' | '/') Power }
double Parser::parseTerm(const std::string& expr, size_t& pos) {
    double value = parsePower(expr, pos);
    while (true) {
        while (pos < expr.size() && isspace(expr[pos])) ++pos;
        if (pos >= expr.size()) break;
        char op = expr[pos];
        if (op != '*' && op != '/') break;
        ++pos;
        double rhs = parsePower(expr, pos);
        if (op == '*') value *= rhs;
        else value /= rhs;
    }
    return value;
}

// Power := Factor { '^' Power }  (RIGHT-ASSOCIATIVE)
double Parser::parsePower(const std::string& expr, size_t& pos) {
    double value = parseFactor(expr, pos);
    while (pos < expr.size()) {
        while (pos < expr.size() && isspace(expr[pos])) ++pos;
        if (pos >= expr.size() || expr[pos] != '^') break;
        ++pos;
        // For right-associativity, recursively call parsePower instead of parseFactor
        double rhs = parsePower(expr, pos);
        value = std::pow(value, rhs);
        break;  // Only one power operation in this recursion level
    }
    return value;
}

// Factor := Number | '(' Expression ')' | Function | Variable | Unary +/-
double Parser::parseFactor(const std::string& expr, size_t& pos) {
    while (pos < expr.size() && isspace(expr[pos])) ++pos;
    if (pos >= expr.size()) throw std::runtime_error("Unexpected end of expression");

    if (expr[pos] == '+') { ++pos; return parseFactor(expr, pos); }
    if (expr[pos] == '-') { ++pos; return -parseFactor(expr, pos); }

    // Function or variable
    if (isalpha(expr[pos])) {
        size_t start = pos;
        while (pos < expr.size() && (isalnum(expr[pos]) || expr[pos] == '_')) ++pos;
        std::string name = expr.substr(start, pos - start);

        // If function call
        while (pos < expr.size() && isspace(expr[pos])) ++pos;
        if (pos < expr.size() && expr[pos] == '(') {
            ++pos;
            double arg = parseExpression(expr, pos);
            if (pos >= expr.size() || expr[pos] != ')')
                throw std::runtime_error("Missing ')' in function call");
            ++pos;

            if (name == "sin") return std::sin(arg);
            if (name == "cos") return std::cos(arg);
            throw std::runtime_error("Unknown function: " + name);
        }

        // Otherwise, variable lookup
        if (variables.find(name) == variables.end())
            throw std::runtime_error("Undefined variable: " + name);
        return variables[name];
    }

    // Parentheses
    if (expr[pos] == '(') {
        ++pos;
        double value = parseExpression(expr, pos);
        if (pos >= expr.size() || expr[pos] != ')')
            throw std::runtime_error("Missing ')'");
        ++pos;
        return value;
    }

    // Number
    return parseNumber(expr, pos);
}

// Parse numbers in binary (b), hex (0x), or decimal
double Parser::parseNumber(const std::string& expr, size_t& pos) {
    while (pos < expr.size() && isspace(expr[pos])) ++pos;

    size_t start = pos;
    // Hex
    if (pos < expr.size() && expr[pos] == '0' && (pos + 1 < expr.size()) &&
        (expr[pos + 1] == 'x' || expr[pos + 1] == 'X')) {
        pos += 2;
        size_t hexStart = pos;
        while (pos < expr.size() && std::isxdigit(expr[pos])) ++pos;
        std::string hexStr = expr.substr(hexStart, pos - hexStart);
        return static_cast<double>(std::stoul(hexStr, nullptr, 16));
    }

    // Binary (ends with 'b')
    while (pos < expr.size() && (expr[pos] == '0' || expr[pos] == '1')) ++pos;
    if (pos < expr.size() && (expr[pos] == 'b' || expr[pos] == 'B')) {
        std::string binStr = expr.substr(start, pos - start);
        ++pos;
        unsigned long val = std::stoul(binStr, nullptr, 2);
        return static_cast<double>(val);
    }

    // Decimal
    pos = start;
    while (pos < expr.size() && (std::isdigit(expr[pos]) || expr[pos] == '.')) ++pos;
    std::string numStr = expr.substr(start, pos - start);
    return std::stod(numStr);
}
