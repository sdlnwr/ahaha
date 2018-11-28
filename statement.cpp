/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

void printSyntaxError()
{
    cout << "SYNTAX ERROR" << endl;
}

PrintStatement::PrintStatement(Expression* exp)
{
    this->exp= exp;
}

PrintStatement::~PrintStatement()
{
    delete exp;
}

void PrintStatement::execute(EvalState & state)
{
    try {
        int value = exp->eval(state);
        cout << value << endl;
    } catch (ErrorException & ex) {
        cout << ex.getMessage() << endl;
    }
}

StatementType PrintStatement::getType()
{
    return EXECUTABLE_STATEMEMT;
}

LetStatement::LetStatement(Expression* exp)
{
    this->exp= exp;
}

LetStatement::~LetStatement()
{
    delete exp;
}

void LetStatement::execute(EvalState & state)
{
    try {
        exp->eval(state);
    } catch (ErrorException & ex) {
        cout << ex.getMessage() << endl;
    }
}

StatementType LetStatement::getType()
{
    return EXECUTABLE_STATEMEMT;
}

InputStatement::InputStatement(Expression *exp)
{
    this->exp = exp;
}

InputStatement::~InputStatement()
{
    delete exp;
}

void InputStatement::execute(EvalState & state)
{
    bool run = true;
    while (run)
    {
        cout << " ? ";
        string line = getLine();
        char* p;
        long converted = strtol(line.c_str(), &p, 10);
        if (*p) {
            cout << "INVALID NUMBER" << endl;
        }
        else {
            state.setValue(((IdentifierExp*)exp)->getName(), converted);
            run = false;
        }
    }
}

StatementType InputStatement::getType()
{
    return EXECUTABLE_STATEMEMT;
}

CommentStatement::CommentStatement() {}

void CommentStatement::execute(EvalState &state)
{
}

StatementType CommentStatement::getType()
{
    return EXECUTABLE_STATEMEMT;
}

EndStatement::EndStatement() {}

void EndStatement::execute(EvalState &state)
{
}

StatementType EndStatement::getType()
{
    return END_STATEMENT;
}


GotoStatement::GotoStatement(int line)
{
    this->line = line;
}

void GotoStatement::execute(EvalState &state) {}

StatementType GotoStatement::getType()
{
    return GOTO_STATEMENT;
}

int GotoStatement::getLineNumber()
{
    return line;
}

IfThenStatement::IfThenStatement(Expression * lexp, Expression * rexp, string cmp,  int line)
{
    this->left_exp = lexp;
    this->right_exp = rexp;
    this->cmp_str = cmp;
    this->line = line;
}

void IfThenStatement::execute(EvalState &state) {}

StatementType IfThenStatement::getType()
{
    return IF_THEN_STATEMENT;
}

bool IfThenStatement::eval(EvalState &state)
{
    bool ok = false;
    try {
        int left_value = left_exp->eval(state);
        int right_value = right_exp->eval(state);
        if (cmp_str.compare("=") == 0)
        {
            if (left_value == right_value)
            {
                ok = true;
            }
        } else if (cmp_str.compare("<") == 0)
        {
            if (left_value < right_value)
            {
                ok = true;
            }
        } else if (cmp_str.compare(">") == 0)
        {
            if (left_value > right_value)
            {
                ok = true;
            }
        }
    } catch (ErrorException & ex) {
        cout << ex.getMessage() << endl;
    }
    return ok;
}

int IfThenStatement::getLineNumber()
{
    return line;
}


IfThenStatement::~IfThenStatement()
{
    delete left_exp;
    delete right_exp;
}
