#ifndef FORMATTER_H
#define FORMATTER_H

#include <string>
#include <iomanip>
#include <sstream>

class Formatter {
public:
    // Format a result based on whether it's an integer or decimal
    static std::string formatResult(const std::string& expression, double result, bool hasDecimal) {
        long long intVal = static_cast<long long>(result);
        
        // Format the result value
        std::string resultStr;
        if (std::abs(result - intVal) < 1e-9 && !hasDecimal) {
            resultStr = std::to_string(intVal);
        } else {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << result;
            resultStr = ss.str();
        }
        
        // Return expression = result on single line
        return expression + " = " + resultStr;
    }
    
    // Check if expression contains a decimal point
    static bool hasDecimalPoint(const std::string& expression) {
        return expression.find('.') != std::string::npos;
    }
};

#endif // FORMATTER_H
