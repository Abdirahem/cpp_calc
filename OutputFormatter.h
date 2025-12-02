#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include <string>
#include <iomanip>
#include <sstream>

class OutputFormatter {
public:
    // Format a result based on whether it's an integer or decimal
    static std::string formatResult(const std::string& expression, double result, bool hasDecimal) {
        long long intVal = static_cast<long long>(result);
        
        // If result is close to integer and input has no decimal, show as integer
        if (std::abs(result - intVal) < 1e-9 && !hasDecimal) {
            return expression + " = " + std::to_string(intVal);
        }
        
        // Otherwise show with 2 decimal places
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << result;
        return expression + " = " + ss.str();
    }
    
    // Check if expression contains a decimal point
    static bool hasDecimalPoint(const std::string& expression) {
        return expression.find('.') != std::string::npos;
    }
};

#endif // OUTPUT_FORMATTER_H
