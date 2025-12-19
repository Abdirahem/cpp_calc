#include <iostream>
#include <string>
#include "include/evaluator.h"
#include "include/file_reader.h"
#include "include/expression_processor.h"
#include "include/result_writer.h"

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <inputFileName>" << std::endl;
    std::cerr << "Example: " << programName << " input.txt" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        // Check command line arguments
        if (argc < 2) {
            printUsage(argv[0]);
            return 1;
        }

        std::string inputFile = argv[1];
        std::string outputFile = "output.txt";

        std::cout << "Reading from: " << inputFile << std::endl;

        // Step 1: Read all expressions from input file
        std::vector<std::string> expressions = FileReader::readExpressions(inputFile);
        std::cout << "Found " << expressions.size() << " expressions" << std::endl;

        // Step 2: Process and evaluate expressions
        Evaluator evaluator;
        CategoryResults results = ExpressionProcessor::processExpressions(expressions, evaluator);

        // Step 3: Write results to output file
        ResultWriter::writeResults(outputFile, results);

        std::cout << "Results written to: " << outputFile << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
