#include <iostream>
#include <iomanip>
#include "include/session_parser.h"
#include "include/formatter.h"
#include "include/evaluator.h"

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
        std::vector<Session> sessions = SessionParser::parseSessions(filename);

        // Print basic summary
        std::cout << SessionParser::getSummary(sessions);

        // Evaluate sessions: a session is "correct" if all its variables and expressions evaluate without error
        int correctSessions = 0;
        std::vector<std::string> sessionReports;

        for (const auto& session : sessions) {
            // Build lists of formatted outputs for variables and expressions
            std::vector<std::string> varLines = session.variables;  // Store raw variable defs
            std::vector<std::string> exprLines;  // Store formatted expression results
            
            Evaluator evaluator; // fresh evaluator per session so variables don't leak between sessions
            bool sessionOK = true;
            std::string report;

            // First, evaluate variable definitions (they may set state)
            for (const auto& varLine : session.variables) {
                try {
                    evaluator.evaluate(varLine);
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
                        double val = evaluator.evaluate(expr);
                        bool hasDecimal = Formatter::hasDecimalPoint(expr);
                        std::string out = Formatter::formatResult(expr, val, hasDecimal);
                        exprLines.push_back(out);
                    } catch (const std::exception& e) {
                        sessionOK = false;
                        exprLines.push_back(expr + " => Error: " + std::string(e.what()));
                        report += "Expression error in session " + std::to_string(session.sessionNumber) + ": '" + expr + "' -> " + e.what() + "\n";
                        break;
                    }
                }
            }

            if (sessionOK) {
                correctSessions++;
                report = "Session " + std::to_string(session.sessionNumber) + " : OK\n";
            }

            // Store report (report contains OK/errors summary)
            sessionReports.push_back(report);
            
            // Print the session block using structured format
            std::cout << "----" << std::endl;
            std::cout << "Session " << session.sessionNumber << std::endl;
            
            if (!varLines.empty()) {
                std::cout << "Variables:" << std::endl;
                for (const auto& v : varLines) {
                    std::cout << "  " << v << std::endl;
                }
            }
            
            if (!exprLines.empty()) {
                std::cout << "Expression:" << std::endl;
                for (const auto& e : exprLines) {
                    std::cout << "  " << e << std::endl;
                }
            }
            std::cout << std::endl;
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
