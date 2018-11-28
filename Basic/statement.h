/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/tokenscanner.h"

enum StatementType {EXECUTABLE_STATEMEMT, END_STATEMENT, GOTO_STATEMENT, IF_THEN_STATEMENT};

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

   virtual StatementType getType() = 0;
};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class PrintStatement : public Statement
{
public:
    PrintStatement(Expression *exp);
    void execute(EvalState & state);
    StatementType getType();
    virtual ~PrintStatement();

private:
    Expression *exp;
};

class LetStatement : public Statement
{
public:
    LetStatement(Expression *exp);
    void execute(EvalState & state);
    StatementType getType();
    virtual ~LetStatement();

private:
    Expression *exp;
};

class InputStatement : public Statement
{
public:
    InputStatement(Expression *exp);
    void execute(EvalState & state);
    StatementType getType();
    virtual ~InputStatement();

private:
    Expression *exp;
};

class CommentStatement : public Statement
{
public:
    CommentStatement();
    void execute(EvalState & state);
    StatementType getType();

private:
};

class EndStatement : public Statement
{
public:
    EndStatement();
    void execute(EvalState & state);
    StatementType getType();

private:
};

class GotoStatement : public Statement
{
public:
    GotoStatement(int line);
    void execute(EvalState & state);
    StatementType getType();
    int getLineNumber();
private:
    int line;
};

class IfThenStatement : public Statement
{
public:
    IfThenStatement(Expression *lexp, Expression *rexp, string cmp, int line);
    void execute(EvalState & state);
    StatementType getType();
    bool eval(EvalState & state);
    int getLineNumber();
    virtual ~IfThenStatement();
private:
    Expression *left_exp;
    Expression *right_exp;
    string cmp_str;
    int line;
};

void printSyntaxError();

#endif
