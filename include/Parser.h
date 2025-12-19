#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>

class Parser {
public:
    Parser();
    
    double parseStatement(const std::string& expr, size_t& pos);
    double parseExpression(const std::string& expr, size_t& pos);
    double parseTerm(const std::string& expr, size_t& pos);
    double parsePower(const std::string& expr, size_t& pos);
    double parseFactor(const std::string& expr, size_t& pos);
    double parseNumber(const std::string& expr, size_t& pos);
    
    // Getter for variables
    std::unordered_map<std::string, double>& getVariables() {
        return variables;
    }

private:
    std::unordered_map<std::string, double> variables;  // store variable values
};

#endif // PARSER_H
