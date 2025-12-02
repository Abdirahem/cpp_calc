#ifndef RESULT_PROCESSOR_H
#define RESULT_PROCESSOR_H

#include <vector>
#include <string>
#include <stdexcept>
#include "calculator.h"
#include "OutputFormatter.h"
#include "ExpressionCategorizer.h"

struct CategoryResults {
    std::vector<std::string> basicCalc;
    std::vector<std::string> hexBinary;
    std::vector<std::string> variables;
    std::vector<std::string> advanced;
};

class ResultProcessor {
public:
    // Process a list of expressions and return categorized results
    static CategoryResults processExpressions(
        const std::vector<std::string>& expressions,
        Calculator& calc) {
        
        CategoryResults results;
        
        for (const auto& expression : expressions) {
            try {
                // Evaluate the expression
                double result = calc.evaluate(expression);
                
                // Format the result
                bool hasDecimal = OutputFormatter::hasDecimalPoint(expression);
                std::string output = OutputFormatter::formatResult(expression, result, hasDecimal);
                
                // Categorize and store
                ExpressionCategorizer::Category cat = ExpressionCategorizer::categorize(expression);
                addToCategory(results, cat, output);
                
            } catch (const std::exception& e) {
                // Handle errors
                std::string errorOutput = expression + " => Error: " + e.what();
                ExpressionCategorizer::Category cat = ExpressionCategorizer::categorize(expression);
                addToCategory(results, cat, errorOutput);
            }
        }
        
        return results;
    }

private:
    // Helper method to add result to appropriate category
    static void addToCategory(CategoryResults& results, 
                             ExpressionCategorizer::Category cat,
                             const std::string& output) {
        switch (cat) {
            case ExpressionCategorizer::BASIC_CALC:
                results.basicCalc.push_back(output);
                break;
            case ExpressionCategorizer::HEX_BINARY:
                results.hexBinary.push_back(output);
                break;
            case ExpressionCategorizer::VARIABLES:
                results.variables.push_back(output);
                break;
            case ExpressionCategorizer::ADVANCED:
                results.advanced.push_back(output);
                break;
        }
    }
};

#endif // RESULT_PROCESSOR_H
