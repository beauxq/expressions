#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

#include <string>
#include <queue>

#include "Token.h"

class Evaluator
{
private:
    static const int FIRST_NON_WHITE;
    static const int LAST_NON_WHITE;

    std::string expression;
    int cursor;

    void store_exp(const std::string& input);
    /** stores string in expression */
    void reset_exp();
    /** resets the cursor */
    int read_int();
    /** reads an integer out of the expression from the cursor */
    void eat_white();
    /** moves cursor to next non-whitespace, printable character or end */
    std::string cursor_str();
    /** returns cursor position for error message */
    std::queue<Token> make_tokens();
    /** tokenizes expression into a queue of operators and operands */
public:
    Evaluator() : expression(""), cursor(0) {}
    Evaluator(const std::string& input) : expression(input), cursor(0) {}

    int eval();
    /** evaluate the expression already stored */

    int eval(const std::string& input);
    /** store a new expression and evaluate it */
};

#endif // EVALUATOR_H_INCLUDED
