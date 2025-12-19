#ifndef RESULT_WRITER_H
#define RESULT_WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "expression_processor.h"
#include "categorizer.h"

class ResultWriter {
public:
    // Write categorized results to output file
    static void writeResults(const std::string& filename, const CategoryResults& results) {
        std::ofstream output(filename);
        if (!output) {
            throw std::runtime_error("Error: could not open output file: " + filename);
        }
        
        writeCategory(output, Categorizer::BASIC_CALC, results.basicCalc);
        writeCategory(output, Categorizer::HEX_BINARY, results.hexBinary);
        writeCategory(output, Categorizer::VARIABLES, results.variables);
        writeCategory(output, Categorizer::ADVANCED, results.advanced);
        
        output.close();
    }

    // Write a single session's results to an already-open output stream
    // Legacy: write categorized results for a session (keeps previous behavior)
    static void writeSession(std::ofstream& output, int sessionNumber, const CategoryResults& results) {
        output << "----" << std::endl;
        output << "Session " << sessionNumber << std::endl;
        writeCategory(output, Categorizer::BASIC_CALC, results.basicCalc);
        writeCategory(output, Categorizer::HEX_BINARY, results.hexBinary);
        writeCategory(output, Categorizer::VARIABLES, results.variables);
        writeCategory(output, Categorizer::ADVANCED, results.advanced);
    }

    // Preferred: write a session block showing header, variable definitions, then expression(s)
    static void writeSession(std::ofstream& output,
                             int sessionNumber,
                             const std::vector<std::string>& variableDefs,
                             const std::vector<std::string>& expressionOutputs) {
        output << "----" << std::endl;
        output << "Session " << sessionNumber << std::endl;

        // Write variable definitions section
        if (!variableDefs.empty()) {
            output << "Variables:" << std::endl;
            for (const auto& v : variableDefs) {
                output << "  " << v << std::endl;
            }
        }

        // Write expression section
        if (!expressionOutputs.empty()) {
            output << "Expression:" << std::endl;
            for (const auto& e : expressionOutputs) {
                output << "  " << e << std::endl;
            }
        }
    }

private:
    // Helper method to write a category if it has results
    static void writeCategory(std::ofstream& output,
                             Categorizer::Category cat,
                             const std::vector<std::string>& items) {
        if (items.empty()) return;
        
        output << Categorizer::getCategoryName(cat) << std::endl;
        for (const auto& item : items) {
            output << "------" << std::endl;
            output << item << std::endl;
        }
        output << std::endl;
    }
};

#endif // RESULT_WRITER_H
