# Calculator Project - README

## Overview

A modular, extensible **expression calculator** written in C++ that supports:
- Basic arithmetic operations (+, -, *, /, ^)
- Variable assignments and lookups
- Trigonometric functions (sin, cos)
- Multiple number formats (decimal, hexadecimal, binary)
- Categorized output organization
- Command-line interface

## Features

### Arithmetic Operations
- Addition: `10 + 5`
- Subtraction: `10 - 5`
- Multiplication: `10 * 5`
- Division: `10 / 5`
- Power (right-associative): `2 ^ 3 ^ 2` = 2^(3^2) = 512

### Number Formats
- **Decimal**: `100`, `3.14`
- **Hexadecimal**: `0x1F` (31 in decimal)
- **Binary**: `1100b` (12 in decimal)

### Variables
- Define: `x = 10`
- Use: `x + 5` → 15
- Variables persist across expressions

### Functions
- `sin(angle)` - sine function
- `cos(angle)` - cosine function

### Expression Examples
```
100                           → 100
0x1F                         → 31
1100b                        → 12
x = 10                       → 10
x + 20                       → 30
2 ^ 3                        → 8
32 / 8 ^ 2                   → 0.50
( 1 + 2 ) * 3               → 9
sin(0)                       → 0
3.14 * radius ^ 2           → 28.26
```

## Project Structure

```
calculator/
├── README.md                     # This file
├── PROJECT_REPORT.md            # Detailed project report
│
├── Core Calculation Engine
│   ├── Parser.h / Parser.cpp     # Expression parsing & evaluation
│   ├── calculator.h / calculator.cpp    # Calculator wrapper
│
├── Input/Output Processing
│   ├── InputReader.h            # Read expressions from file
│   ├── ResultProcessor.h         # Process & categorize results
│   ├── OutputWriter.h           # Write results to file
│   ├── OutputFormatter.h        # Format output values
│   ├── ExpressionCategorizer.h  # Categorize expressions
│
├── Session Analysis
│   ├── SessionCounter.h         # Parse and count sessions
│   ├── session_analyzer.cpp     # Session analysis tool
│
├── Entry Point
│   └── main.cpp                 # Command-line interface
│
├── Data Files
│   ├── input.txt                # Input expressions
│   ├── output.txt               # Categorized results
│   └── sessions.txt             # Session definitions
```

## Compilation

### Quick Compile
```bash
g++ -o calculator main.cpp calculator.cpp Parser.cpp
```

### Compile with Session Analyzer
```bash
g++ -o calculator main.cpp calculator.cpp Parser.cpp
g++ -o session_analyzer session_analyzer.cpp
```

## Usage

### Calculator

**Format:**
```bash
./calculator <inputFileName>
```

**Examples:**
```bash
# Process input.txt
./calculator input.txt

# Process custom file
./calculator expressions.txt

# Process sessions
./calculator sessions.txt
```

**Output:**
```
Reading from: input.txt
Found 27 expressions
Results written to: output.txt
```

### Session Analyzer

**Format:**
```bash
./session_analyzer <sessionFileName>
```

**Example:**
```bash
./session_analyzer sessions.txt
```

**Output:**
```
=== SESSION ANALYSIS ===
Total Sessions: 5
Total Variables: 10
Total Expressions: 6

--- Session 1 ---
Variables: 2
Expressions: 1
```

## Input File Format

### For Calculator (input.txt)

```
100
0x1F
1100b

i=10
i
sum = 20
sum + 10

2^3

sin(0)
cos(0)

10 + 2 * 3 - 5
32 / 8 ^ 2
( 1 + 2 ) * 3
```

### For Session Analyzer (sessions.txt)

```
----
x = 10
y = 20
x + y

----
a = 0x1F
b = 1100b
a * b

----
pi = 3.14
radius = 5
pi * radius ^ 2
```

## Output Format

Results are automatically categorized into:

### 1. BASIC CALCULATIONS
```
=== BASIC CALCULATIONS ===
100 = 100
10 + 2 * 3 - 5 = 11
```

### 2. HEX & BINARY
```
=== HEX & BINARY ===
0x1F = 31
1100b = 12
```

### 3. VARIABLES
```
=== VARIABLES ===
x = 10
y = 20
x + y = 30
```

### 4. ADVANCED (POWER & FUNCTIONS)
```
=== ADVANCED (POWER & FUNCTIONS) ===
2^3 = 8
sin(0) = 0
3.14 * radius ^ 2 = 28.26
```

## Key Design Principles

### 1. **Modularity**
- Each component has a single responsibility
- Easy to add new features
- Clean separation of concerns

### 2. **Extensibility**
- Add new functions: Modify `Parser::parseFactor()`
- Add new operators: Modify `Parser::parseTerm()`
- Add new number formats: Modify `Parser::parseNumber()`

### 3. **Correctness**
- Right-associative power operator: `2^3^2` = 512
- Proper operator precedence: `+`, `-` < `*`, `/` < `^`
- Accurate decimal handling

### 4. **User-Friendly**
- Clear categorized output
- Helpful error messages
- Command-line interface

## Operator Precedence (Lowest to Highest)

1. Addition, Subtraction: `+`, `-`
2. Multiplication, Division: `*`, `/`
3. Power (right-associative): `^`
4. Unary: `+`, `-`
5. Parentheses & Functions: `()`, `sin()`, `cos()`

## Examples

### Basic Arithmetic
```
Input: 10 + 2 * 3 - 5
Output: 11
(2*3=6, 10+6=16, 16-5=11)
```

### Power Operation
```
Input: 2 ^ 3 ^ 2
Output: 512
(3^2=9, 2^9=512)
```

### Variables
```
Input: x = 10
       y = 20
       x + y * 2
Output: x = 10
        y = 20
        x + y * 2 = 50
```

### Complex Expression
```
Input: pi = 3.14
       radius = 5
       sin(pi/2) + pi * radius ^ 2
Output: pi = 3.14
        radius = 5
        sin(pi/2) + pi * radius ^ 2 = 79.00
```

## Error Handling

The calculator provides clear error messages:

```
Error: Undefined variable: z
Error: Missing ')' in function call
Error: Unknown function: tan
```

## Testing

All expressions have been tested and verified including:
- ✅ Basic calculations
- ✅ Hex and binary conversions
- ✅ Variable assignments and operations
- ✅ Advanced operations with correct precedence
- ✅ Trigonometric functions
- ✅ Complex nested expressions

## Future Enhancements

Possible additions:
- [ ] More trigonometric functions (tan, asin, acos, atan)
- [ ] Logarithmic functions (log, ln)
- [ ] More operators (%, modulo)
- [ ] User-defined functions
- [ ] Expression history
- [ ] Interactive mode
- [ ] Multi-line expressions

## Building & Running Quick Start

```bash
# Navigate to project
cd /Users/hem/Desktop/calculator

# Compile
g++ -o calculator main.cpp calculator.cpp Parser.cpp

# Run with input file
./calculator input.txt

# Check results
cat output.txt
```

## License

This is an educational project.

## Author

Created: November 2025

## Support

For issues or improvements, review the modular structure:
- Parsing logic: `Parser.cpp`
- Input/Output: `InputReader.h`, `OutputWriter.h`
- Categorization: `ExpressionCategorizer.h`
- Formatting: `OutputFormatter.h`
