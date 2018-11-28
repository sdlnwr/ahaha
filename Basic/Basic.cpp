/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

bool processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   Program program;
   EvalState state;
//   cout << "Stub implementation of BASIC" << endl;
   bool run = true;
   while (run) {
      try {
         run = processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

Statement* parseStatement(TokenScanner &scanner)
{
    string statementName = scanner.nextToken();
    Statement* statement = NULL;
    if (statementName.compare("PRINT") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            try {
                Expression *exp = parseExp(scanner);
                statement = new PrintStatement(exp);
            } catch (ErrorException & ex) {
            }
        }
    } else if (statementName.compare("LET") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            try {
                Expression *exp = parseExp(scanner);
                statement = new LetStatement(exp);
            } catch (ErrorException & ex) {
            }
        }
    } else if (statementName.compare("INPUT") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            try {
                Expression *exp = parseExp(scanner);
                if (exp->getType() == IDENTIFIER)
                {
                    statement = new InputStatement(exp);
                } else
                {
                    delete exp;
                }
            } catch (ErrorException & ex) {
            }
        }
    } else if (statementName.compare("REM") == 0)
    {
        statement = new CommentStatement();
    } else if (statementName.compare("END") == 0)
    {
        if (!scanner.hasMoreTokens())
        {
            statement = new EndStatement();
        }
    } else if (statementName.compare("GOTO") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            string token = scanner.nextToken();
            if (scanner.hasMoreTokens() == false)
            {
                char* p;
                long converted = strtol(token.c_str(), &p, 10);
                if (!(*p))
                {
                    statement = new GotoStatement(converted);
                }
            }
        }
    } else if (statementName.compare("IF") == 0)
    {
        string exp_str1, exp_str2, cmp_str;
        bool meet_cmp = false;
        bool meet_then = false;
        while (scanner.hasMoreTokens())
        {
            string token = scanner.nextToken();
            if (token.compare("<") == 0 || token.compare("=") == 0 || token.compare(">") == 0)
            {
                meet_cmp = true;
                cmp_str = token;
                break;
            }
            exp_str1 += token;
        }
        if (meet_cmp)
        {
            while (scanner.hasMoreTokens())
            {
                string token = scanner.nextToken();
                if (token.compare("THEN") == 0)
                {
                    meet_then = true;
                    break;
                }
                exp_str2 += token;
            }
            if (meet_then)
            {
                string token = scanner.nextToken();
                if (scanner.hasMoreTokens() == false)
                {
                    char* p;
                    long converted = strtol(token.c_str(), &p, 10);
                    if (!(*p))
                    {
                        scanner.setInput(exp_str1);
                        Expression *exp1 = parseExp(scanner);
                        scanner.setInput(exp_str2);
                        Expression *exp2 = parseExp(scanner);
                        statement = new IfThenStatement(exp1, exp2, cmp_str, converted);
                    }
                }
            }
        }
    }
    return statement;
}

bool processLine(string line, Program & program, EvalState & state) {
    bool run = true;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string first_token = scanner.nextToken();
    if (first_token.compare("QUIT") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            printSyntaxError();
        } else
        {
            run = false;
        }
    } else if (first_token.compare("LIST") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            printSyntaxError();
        } else
        {
            for (int lineNumber = program.getFirstLineNumber(); lineNumber != -1; lineNumber = program.getNextLineNumber(lineNumber))
            {
                cout << program.getSourceLine(lineNumber) << endl;
            }
        }
    } else if (first_token.compare("CLEAR") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            printSyntaxError();
        } else
        {
            program.clear();
            state.clear();
        }
    } else if (first_token.compare("RUN") == 0)
    {
        if (scanner.hasMoreTokens())
        {
            printSyntaxError();
        } else
        {
            for (int lineNumber = program.getFirstLineNumber(); lineNumber != -1;)
            {
                Statement *statement = program.getParsedStatement(lineNumber);
                if (statement->getType() == EXECUTABLE_STATEMEMT)
                {
                    statement->execute(state);
                    lineNumber = program.getNextLineNumber(lineNumber);
                } else if (statement->getType() == END_STATEMENT)
                {
                    lineNumber = -1;
                } else if (statement->getType() == GOTO_STATEMENT)
                {
                    int next_step = ((GotoStatement*)statement)->getLineNumber();
                    if (program.getParsedStatement(next_step))
                    {
                        lineNumber = next_step;
                    } else
                    {
                        cout << "LINE NUMBER ERROR" << endl;
                        lineNumber = program.getNextLineNumber(lineNumber);
                    }
                } else if (statement->getType() == IF_THEN_STATEMENT)
                {
                    if (((IfThenStatement*)statement)->eval(state))
                    {
                        int next_step = ((IfThenStatement*)statement)->getLineNumber();
                        if (program.getParsedStatement(next_step))
                        {
                            lineNumber = next_step;
                        } else
                        {
                            cout << "LINE NUMBER ERROR" << endl;
                            lineNumber = program.getNextLineNumber(lineNumber);
                        }
                    } else
                    {
                        lineNumber = program.getNextLineNumber(lineNumber);
                    }
                }
            }
        }
    } else if (first_token.compare("PRINT") == 0 || first_token.compare("LET") == 0 || first_token.compare("INPUT") == 0 || first_token.compare("REM") == 0)
    {
        scanner.saveToken(first_token);
        Statement* statement = parseStatement(scanner);
        if (statement == NULL)
        {
            printSyntaxError();
        } else
        {
            statement->execute(state);
            delete statement;
        }
    } else if (scanner.getTokenType(first_token) == NUMBER)
    {
        int lineNumber = atoi(first_token.c_str());
        if (scanner.hasMoreTokens() == false)
        {
            program.removeSourceLine(lineNumber);
        } else
        {
            Statement* statement = parseStatement(scanner);
            if (statement == NULL)
            {
                printSyntaxError();
            } else
            {
                program.addSourceLine(lineNumber, line);
                program.setParsedStatement(lineNumber, statement);
            }
        }
    } else
    {
        printSyntaxError();
    }
    return run;
}
