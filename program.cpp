/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
   // Replace this stub with your own code
}

Program::~Program() {
   // Replace this stub with your own code
   clear();
}

void Program::clear() {
   // Replace this stub with your own code
   sourceLines.clear();
   for (map<int, Statement*>::iterator i = statements.begin(); i != statements.end(); i++)
   {
       delete i->second;
   }
   statements.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
   // Replace this stub with your own code
   sourceLines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
   if (sourceLines.find(lineNumber) != sourceLines.end())
   {
       sourceLines.erase(lineNumber);
       delete statements.find(lineNumber)->second;
       statements.erase(lineNumber);
   }
}

string Program::getSourceLine(int lineNumber) {
   // Replace this stub with your own code
   return sourceLines[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
   if (statements.find(lineNumber) != statements.end())
   {
       delete statements.find(lineNumber)->second;
   }
   statements[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   if (statements.find(lineNumber) != statements.end())
   {
       return statements[lineNumber];
   } else
   {
       return NULL;
   }
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    if (sourceLines.size() != 0)
    {
        return sourceLines.begin()->first;
    } else
    {
        return -1;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    if (sourceLines.lower_bound(lineNumber + 1) != sourceLines.end())
    {
        return sourceLines.lower_bound(lineNumber + 1)->first;
    } else
    {
        return -1;
    }
}
