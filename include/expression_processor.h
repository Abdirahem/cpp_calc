#ifndef EXPRESSION_PROCESSOR_H
#define EXPRESSION_PROCESSOR_H

#include <vector>
#include <string>
#include <stdexcept>
#include "evaluator.h"
#include "formatter.h"
#include "categorizer.h"

struct CategoryResults {
    std::vector<std::string> basicCalc;
    std::vector<std::string> hexBinary;
    std::vector<std::string> variables;
    std::vector<std::string> advanced;
};

class ExpressionProcessor {
public:
    // Process a list of expressions and return categorized results
    static CategoryResults processExpressions(
        const std::vector<std::string>& expressions,
        Evaluator& evaluator) {
        
        CategoryResults results;
        
        for (const auto& expression : expressions) {
            try {
                // Evaluate the expression (even if it's a variable assignment)
                double result = evaluator.evaluate(expression);
                
                // Skip display of pure variable assignments (e.g., "x = 10")
                // Only show variable usage (e.g., "x + y")
                if (Categorizer::isVariableAssignment(expression)) {
                    continue;
                }
                
                // Format the result
                bool hasDecimal = Formatter::hasDecimalPoint(expression);
                std::string output = Formatter::formatResult(expression, result, hasDecimal);
                
                // Categorize and store
                Categorizer::Category cat = Categorizer::categorize(expression);
                addToCategory(results, cat, output);
                
            } catch (const std::exception& e) {
                // Handle errors
                std::string errorOutput = expression + " => Error: " + e.what();
                Categorizer::Category cat = Categorizer::categorize(expression);
                addToCategory(results, cat, errorOutput);
            }
        }
        
        return results;
    }

private:
    // Helper method to add result to appropriate category
    static void addToCategory(CategoryResults& results, 
                             Categorizer::Category cat,
                             const std::string& output) {
        switch (cat) {
            case Categorizer::BASIC_CALC:
                results.basicCalc.push_back(output);
                break;
            case Categorizer::HEX_BINARY:
                results.hexBinary.push_back(output);
                break;
            case Categorizer::VARIABLES:
                results.variables.push_back(output);
                break;
            case Categorizer::ADVANCED:
                results.advanced.push_back(output);
                break;
        }
    }
};

#endif // EXPRESSION_PROCESSOR_H
