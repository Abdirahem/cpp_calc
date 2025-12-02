#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <fstream>
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

class FileProcessor {
public:
    // Process input file and categorize results
    static CategoryResults processFile(const std::string& inputFile, Calculator& calc) {
        std::ifstream input(inputFile);
        if (!input) {
            throw std::runtime_error("Error: could not open input file.");
        }
        
        CategoryResults results;
        std::string expression;
        
        while (std::getline(input, expression)) {
            if (expression.empty()) continue;
            
            try {
                double result = calc.evaluate(expression);
                bool hasDecimal = OutputFormatter::hasDecimalPoint(expression);
                std::string output = OutputFormatter::formatResult(expression, result, hasDecimal);
                
                // Categorize and store
                ExpressionCategorizer::Category cat = ExpressionCategorizer::categorize(expression);
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
            } catch (const std::exception& e) {
                std::string errorOutput = expression + " => Error: " + e.what();
                ExpressionCategorizer::Category cat = ExpressionCategorizer::categorize(expression);
                switch (cat) {
                    case ExpressionCategorizer::BASIC_CALC:
                        results.basicCalc.push_back(errorOutput);
                        break;
                    case ExpressionCategorizer::HEX_BINARY:
                        results.hexBinary.push_back(errorOutput);
                        break;
                    case ExpressionCategorizer::VARIABLES:
                        results.variables.push_back(errorOutput);
                        break;
                    case ExpressionCategorizer::ADVANCED:
                        results.advanced.push_back(errorOutput);
                        break;
                }
            }
        }
        
        return results;
    }
    
    // Write categorized results to output file
    static void writeResults(const std::string& outputFile, const CategoryResults& results) {
        std::ofstream output(outputFile);
        if (!output) {
            throw std::runtime_error("Error: could not open output file.");
        }
        
        if (!results.basicCalc.empty()) {
            output << ExpressionCategorizer::getCategoryName(ExpressionCategorizer::BASIC_CALC) << std::endl;
            for (const auto& line : results.basicCalc) {
                output << line << std::endl;
            }
            output << std::endl;
        }
        
        if (!results.hexBinary.empty()) {
            output << ExpressionCategorizer::getCategoryName(ExpressionCategorizer::HEX_BINARY) << std::endl;
            for (const auto& line : results.hexBinary) {
                output << line << std::endl;
            }
            output << std::endl;
        }
        
        if (!results.variables.empty()) {
            output << ExpressionCategorizer::getCategoryName(ExpressionCategorizer::VARIABLES) << std::endl;
            for (const auto& line : results.variables) {
                output << line << std::endl;
            }
            output << std::endl;
        }
        
        if (!results.advanced.empty()) {
            output << ExpressionCategorizer::getCategoryName(ExpressionCategorizer::ADVANCED) << std::endl;
            for (const auto& line : results.advanced) {
                output << line << std::endl;
            }
            output << std::endl;
        }
    }
};

#endif // FILE_PROCESSOR_H
