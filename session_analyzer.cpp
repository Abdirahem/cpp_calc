#include <iostream>
#include <iomanip>
#include "SessionCounter.h"
#include "OutputFormatter.h"
#include "calculator.h"

int main(int argc, char* argv[]) {
    try {
    // Use provided filename or default
    std::string filename = (argc > 1) ? argv[1] : "sessions.txt";

    std::cout << "Analyzing sessions from: " << filename << std::endl << std::endl;

    // Print the expression grammar as requested
    std::cout << "expression := number | \"(\" expression \")\" | expression operator expression | function \"(\" expression \")\"" << std::endl;
    std::cout << "operator := \"+\" | \"-\" | \"*\" | \"/\" | \"^\"" << std::endl;
    std::cout << "function := \"sin\" | \"cos\"" << std::endl << std::endl;

        // Parse sessions
        std::vector<Session> sessions = SessionCounter::parseSessions(filename);

        // Print basic summary
        std::cout << SessionCounter::getSummary(sessions);

        // Evaluate sessions: a session is "correct" if all its variables and expressions evaluate without error
        int correctSessions = 0;
        std::vector<std::string> sessionReports;

        for (const auto& session : sessions) {
            Calculator calc; // fresh calculator per session so variables don't leak between sessions
            bool sessionOK = true;
            std::string report;

            // First, evaluate variable definitions (they may set state)
            for (const auto& varLine : session.variables) {
                try {
                    calc.evaluate(varLine);
                } catch (const std::exception& e) {
                    sessionOK = false;
                    report += "Variable error in session " + std::to_string(session.sessionNumber) + ": '" + varLine + "' -> " + e.what() + "\n";
                    break;
                }
            }

            // If variables OK, evaluate expressions
            if (sessionOK) {
                for (const auto& expr : session.expressions) {
                    try {
                        (void)calc.evaluate(expr);
                    } catch (const std::exception& e) {
                        sessionOK = false;
                        report += "Expression error in session " + std::to_string(session.sessionNumber) + ": '" + expr + "' -> " + e.what() + "\n";
                        break;
                    }
                }
            }

            // Build a printable session block: header, variable defs, then expression(s) with results/errors
            std::string sessionBlock;
            sessionBlock += "----\n";
            // Append variable definitions
            for (const auto& v : session.variables) {
                sessionBlock += v + "\n";
            }

            // Evaluate expressions again to capture formatted outputs (if they succeeded earlier)
            for (const auto& expr : session.expressions) {
                try {
                    double val = calc.evaluate(expr);
                    bool hasDecimal = OutputFormatter::hasDecimalPoint(expr);
                    std::string out = OutputFormatter::formatResult(expr, val, hasDecimal);
                    sessionBlock += out + "\n";
                } catch (const std::exception& e) {
                    sessionBlock += expr + " => Error: " + std::string(e.what()) + "\n";
                }
            }

            if (sessionOK) {
                correctSessions++;
                report = "Session " + std::to_string(session.sessionNumber) + " : OK\n";
            }

            // Store report and block (report contains OK/errors summary)
            sessionReports.push_back(report);
            // Print the session block to stdout as part of the analyzer output
            std::cout << sessionBlock << std::endl;
        }

        // Print evaluation summary
        std::cout << "=== SESSION EVALUATION ===" << std::endl;
        std::cout << "Sessions correct: " << correctSessions << " / " << sessions.size() << std::endl << std::endl;

        // Print per-session reports (errors or OK)
        for (const auto& rep : sessionReports) {
            std::cout << rep << std::endl;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
