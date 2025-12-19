#ifndef SESSION_PARSER_H
#define SESSION_PARSER_H

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

class SessionParser {
public:
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
        bool awaitingExpression = false;
        std::vector<std::string> outsideLines;
        
        while (std::getline(file, line)) {
            line = trim(line);
            
            if (line.empty()) continue;
            
            if (line == "----") {
                if (inSession && sessionNum > 0) {
                    sessions.push_back(currentSession);
                }

                sessionNum++;
                currentSession = Session{
                    sessionNum,
                    0,
                    0,
                    std::vector<std::string>(),
                    std::vector<std::string>()
                };
                inSession = true;
                awaitingExpression = true;
                continue;
            }

            if (!inSession) {
                outsideLines.push_back(line);
                continue;
            }

            if (awaitingExpression && line.find('=') != std::string::npos) {
                currentSession.variables.push_back(line);
                currentSession.variableCount++;
                continue;
            }

            if (awaitingExpression && !line.empty()) {
                currentSession.expressions.push_back(line);
                currentSession.expressionCount = 1;
                sessions.push_back(currentSession);
                inSession = false;
                awaitingExpression = false;
            }
        }
        
        if (inSession && sessionNum > 0) {
            sessions.push_back(currentSession);
        }

        if (sessions.empty() && !outsideLines.empty()) {
            size_t i = 0;
            const size_t n = outsideLines.size();
            while (i < n) {
                std::vector<std::string> vars;
                while (i < n && outsideLines[i].find('=') != std::string::npos) {
                    vars.push_back(outsideLines[i]);
                    ++i;
                }

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
    
    static int countSessions(const std::vector<Session>& sessions) {
        return sessions.size();
    }
    
    static int countTotalVariables(const std::vector<Session>& sessions) {
        int total = 0;
        for (const auto& session : sessions) {
            total += session.variableCount;
        }
        return total;
    }
    
    static int countTotalExpressions(const std::vector<Session>& sessions) {
        int total = 0;
        for (const auto& session : sessions) {
            total += session.expressionCount;
        }
        return total;
    }
    
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
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
};

#endif // SESSION_PARSER_H
