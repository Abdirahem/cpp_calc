#ifndef EXPRESSION_CATEGORIZER_H
#define EXPRESSION_CATEGORIZER_H

#include <string>

class ExpressionCategorizer {
public:
    enum Category {
        BASIC_CALC,
        HEX_BINARY,
        VARIABLES,
        ADVANCED
    };
    
    // Categorize an expression based on its content
    static Category categorize(const std::string& expression) {
        // Check for advanced operations first (functions, power, parentheses)
        if (expression.find('(') != std::string::npos || 
            expression.find('^') != std::string::npos ||
            expression.find("sin") != std::string::npos ||
            expression.find("cos") != std::string::npos) {
            return ADVANCED;
        }
        
        // Check for variable assignments
        if (expression.find('=') != std::string::npos || 
            (std::isalpha(expression[0]) && expression.find_first_of("+-*/%") != std::string::npos)) {
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
                return "=== BASIC CALCULATIONS ===";
            case HEX_BINARY:
                return "=== HEX & BINARY ===";
            case VARIABLES:
                return "=== VARIABLES ===";
            case ADVANCED:
                return "=== ADVANCED (POWER & FUNCTIONS) ===";
            default:
                return "=== UNKNOWN ===";
        }
    }
};

#endif // EXPRESSION_CATEGORIZER_H
