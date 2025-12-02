#ifndef SESSION_COUNTER_H
#define SESSION_COUNTER_H

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

struct Session {
    int sessionNumber;
    int variableCount;
    int expressionCount;
    std::vector<std::string> variables;
    std::vector<std::string> expressions;
};

class SessionCounter {
public:
    // Parse sessions from a file and count them
    static std::vector<Session> parseSessions(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Error: could not open file: " + filename);
        }
        
    std::vector<Session> sessions;
        std::string line;
        int sessionNum = 0;
        Session currentSession;
        bool inSession = false;
        bool awaitingExpression = false; // per grammar: after vars expect one expression
    std::vector<std::string> outsideLines;
        
        while (std::getline(file, line)) {
            // Trim whitespace
            line = trim(line);
            
            // Skip empty lines
            if (line.empty()) continue;
            
            // Check for session header "----"
            if (line == "----") {
                // If we were in a previous session that was awaiting an expression, we close it (no expression found)
                if (inSession && sessionNum > 0) {
                    sessions.push_back(currentSession);
                }

                // Start new session
                sessionNum++;
                currentSession = Session{
                    sessionNum,
                    0,
                    0,
                    std::vector<std::string>(),
                    std::vector<std::string>()
                };
                inSession = true;
                awaitingExpression = true; // by grammar, after header expect vars then one expression
                continue;
            }

            if (!inSession) {
                // collect lines outside sessions; if the file contains no session headers we'll
                // treat each such line as its own session later
                outsideLines.push_back(line);
                continue;
            }

            // If line contains '=' and we haven't seen the expression yet, it's a variable definition
            if (awaitingExpression && line.find('=') != std::string::npos) {
                currentSession.variables.push_back(line);
                currentSession.variableCount++;
                continue;
            }

            // Otherwise, if awaitingExpression and this line is not a variable, treat it as the single expression of this session
            if (awaitingExpression && !line.empty()) {
                currentSession.expressions.push_back(line);
                currentSession.expressionCount = 1;
                // session complete (as per grammar)
                sessions.push_back(currentSession);
                // reset session state
                inSession = false;
                awaitingExpression = false;
            }
        }
        
        // Don't forget the last session
        if (inSession && sessionNum > 0) {
            sessions.push_back(currentSession);
        }

        // If we found no explicit sessions (no "----" headers), interpret the file
        // using the grammar: zero-or-more variable_definitions followed by exactly one expression.
        // So group contiguous variable-definition lines (lines containing '=') with the
        // following non-variable line as that session's single expression. This fixes
        // cases where a variable is defined on one line and used on the next.
        if (sessions.empty() && !outsideLines.empty()) {
            size_t i = 0;
            const size_t n = outsideLines.size();
            while (i < n) {
                // Collect zero-or-more variable definition lines
                std::vector<std::string> vars;
                while (i < n && outsideLines[i].find('=') != std::string::npos) {
                    vars.push_back(outsideLines[i]);
                    ++i;
                }

                // Next line (if present) is the single expression for this session
                if (i < n) {
                    std::string expr = outsideLines[i];
                    ++i;
                    sessionNum++;
                    Session s{
                        sessionNum,
                        static_cast<int>(vars.size()),
                        1,
                        vars,
                        std::vector<std::string>{expr}
                    };
                    sessions.push_back(s);
                } else {
                    // Reached EOF after variable definitions without an expression.
                    // Per grammar, there should be an expression; create a session with
                    // zero expressions (variable-only session) to preserve information.
                    sessionNum++;
                    Session s{
                        sessionNum,
                        static_cast<int>(vars.size()),
                        0,
                        vars,
                        std::vector<std::string>()
                    };
                    sessions.push_back(s);
                }
            }
        }
        
        file.close();
        return sessions;
    }
    
    // Count total sessions
    static int countSessions(const std::vector<Session>& sessions) {
        return sessions.size();
    }
    
    // Count total variables across all sessions
    static int countTotalVariables(const std::vector<Session>& sessions) {
        int total = 0;
        for (const auto& session : sessions) {
            total += session.variableCount;
        }
        return total;
    }
    
    // Count total expressions across all sessions
    static int countTotalExpressions(const std::vector<Session>& sessions) {
        int total = 0;
        for (const auto& session : sessions) {
            total += session.expressionCount;
        }
        return total;
    }
    
    // Get session summary
    static std::string getSummary(const std::vector<Session>& sessions) {
        std::ostringstream ss;
        
        ss << "=== SESSION ANALYSIS ===" << std::endl << std::endl;
        ss << "Total Sessions: " << sessions.size() << std::endl;
        ss << "Total Variables: " << countTotalVariables(sessions) << std::endl;
        ss << "Total Expressions: " << countTotalExpressions(sessions) << std::endl;
        ss << std::endl;
        
        for (const auto& session : sessions) {
            ss << "--- Session " << session.sessionNumber << " ---" << std::endl;
            ss << "Variables: " << session.variableCount << std::endl;
            ss << "Expressions: " << session.expressionCount << std::endl;
            ss << std::endl;
        }
        
        return ss.str();
    }

private:
    // Trim whitespace from string
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
};

#endif // SESSION_COUNTER_H
