#ifndef CATEGORIZER_H
#define CATEGORIZER_H

#include <string>

class Categorizer {
public:
    enum Category {
        BASIC_CALC,
        HEX_BINARY,
        VARIABLES,
        ADVANCED
    };
    
    // Check if expression is a variable assignment (e.g., "x = 10")
    static bool isVariableAssignment(const std::string& expression) {
        size_t eqPos = expression.find('=');
        if (eqPos == std::string::npos) return false;
        
        // Check if the part before '=' is a simple variable name
        std::string lhs = expression.substr(0, eqPos);
        
        // Trim whitespace
        size_t start = lhs.find_first_not_of(" \t");
        size_t end = lhs.find_last_not_of(" \t");
        
        if (start == std::string::npos) return false;
        
        lhs = lhs.substr(start, end - start + 1);
        
        // Check if it's a valid variable name (alphanumeric and underscore only, no operators)
        if (lhs.empty() || (!std::isalpha(lhs[0]) && lhs[0] != '_')) return false;
        
        for (size_t i = 0; i < lhs.length(); i++) {
            if (!std::isalnum(lhs[i]) && lhs[i] != '_') return false;
        }
        
        return true;
    }
    
    // Categorize an expression based on its content
    static Category categorize(const std::string& expression) {
        // Check for variable assignment first (x = value)
        if (isVariableAssignment(expression)) {
            return BASIC_CALC;  // Assignments are basic but will be hidden in output
        }
        
        // Check for advanced operations (functions, power, parentheses)
        if (expression.find('(') != std::string::npos || 
            expression.find('^') != std::string::npos ||
            expression.find("sin") != std::string::npos ||
            expression.find("cos") != std::string::npos) {
            return ADVANCED;
        }
        
        // Check for variable usage (using variables in expressions)
        if (std::isalpha(expression[0]) && expression.find_first_of("+-*/%") != std::string::npos) {
            return VARIABLES;
        }
        
        // Check for hex notation
        if (expression.find("0x") != std::string::npos) {
            return HEX_BINARY;
        }
        
        // Check for binary notation
        if (expression.find('b') != std::string::npos && 
            (expression.find('1') != std::string::npos || expression.find('0') != std::string::npos)) {
            return HEX_BINARY;
        }
        
        // Default to basic calculation
        return BASIC_CALC;
    }
    
    // Get category name as string
    static std::string getCategoryName(Category cat) {
        switch (cat) {
            case BASIC_CALC:
                return "\n***************** BASIC CALCULATIONS *******\n";
            case HEX_BINARY:
                return "\n\n***************** HEX & BINARY *******\n";
            case VARIABLES:
                return "\n\n*****************   VARIABLES *******\n";
            case ADVANCED:
                return "\n\n***************** ADVANCED (POWER & FUNCTIONS) *******\n";
            default:
                return "\n\n***************** UNKNOWN *******\n";
        }
    }
};

#endif // CATEGORIZER_H
