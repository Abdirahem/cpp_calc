#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "ResultProcessor.h"
#include "ExpressionCategorizer.h"

class OutputWriter {
public:
    // Write categorized results to output file
    static void writeResults(const std::string& filename, const CategoryResults& results) {
        std::ofstream output(filename);
        if (!output) {
            throw std::runtime_error("Error: could not open output file: " + filename);
        }
        
        writeCategory(output, ExpressionCategorizer::BASIC_CALC, results.basicCalc);
        writeCategory(output, ExpressionCategorizer::HEX_BINARY, results.hexBinary);
        writeCategory(output, ExpressionCategorizer::VARIABLES, results.variables);
        writeCategory(output, ExpressionCategorizer::ADVANCED, results.advanced);
        
        output.close();
    }

    // Write a single session's results to an already-open output stream
    // Legacy: write categorized results for a session (keeps previous behavior)
    static void writeSession(std::ofstream& output, int sessionNumber, const CategoryResults& results) {
        output << "----" << std::endl;
        output << "Session " << sessionNumber << std::endl;
        writeCategory(output, ExpressionCategorizer::BASIC_CALC, results.basicCalc);
        writeCategory(output, ExpressionCategorizer::HEX_BINARY, results.hexBinary);
        writeCategory(output, ExpressionCategorizer::VARIABLES, results.variables);
        writeCategory(output, ExpressionCategorizer::ADVANCED, results.advanced);
    }

    // Preferred: write a session block showing header, variable definitions, then expression(s)
    static void writeSession(std::ofstream& output,
                             int sessionNumber,
                             const std::vector<std::string>& variableDefs,
                             const std::vector<std::string>& expressionOutputs) {
        output << "----" << std::endl;
        // Optionally label the session
        output << "Session " << sessionNumber << std::endl;

        // Write variable definitions first (raw lines)
        for (const auto& v : variableDefs) {
            output << v << std::endl;
        }

        // Then write expressions (already formatted with results or errors)
        for (const auto& e : expressionOutputs) {
            output << e << std::endl;
        }
    }

private:
    // Helper method to write a category if it has results
    static void writeCategory(std::ofstream& output,
                             ExpressionCategorizer::Category cat,
                             const std::vector<std::string>& items) {
        if (items.empty()) return;
        
        output << ExpressionCategorizer::getCategoryName(cat) << std::endl;
        for (const auto& item : items) {
            output << item << std::endl;
        }
        output << std::endl;
    }
};

#endif // OUTPUT_WRITER_H
