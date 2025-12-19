#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

class FileReader {
public:
    // Read all expressions from input file
    static std::vector<std::string> readExpressions(const std::string& filename) {
        std::ifstream input(filename);
        if (!input) {
            throw std::runtime_error("Error: could not open input file: " + filename);
        }
        
        std::vector<std::string> expressions;
        std::string line;
        
        while (std::getline(input, line)) {
            // Trim whitespace
            line = trim(line);
            
            // Skip empty lines
            if (line.empty()) continue;
            
            // Skip session headers (----)
            if (line == "----") continue;
            
            expressions.push_back(line);
        }
        
        input.close();
        return expressions;
    }
    
private:
    // Helper to trim whitespace
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
};

#endif // FILE_READER_H
