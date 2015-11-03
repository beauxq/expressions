#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

#include <string>
#include <queue>
#include <stack>

#include "Token.h"

class Evaluator
{
private:
    static const int FIRST_NON_WHITE;

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
    std::string cursor_str(int where = -1) const;
    /** returns cursor position for error message */
    std::queue<Token> make_tokens();
    /** tokenizes expression into a queue of operators and operands */
    void process_operator(std::stack<int>& operands, std::stack<Token>& operators, const Token& op) const;
    /** TODO document this */
    int eval_tokens(std::queue<Token>& tokens) const;
    /** evaluate the expression from quque of tokens */
    int evaluate_one_operator(std::stack<int>& operands, const Token& operator_) const;
    /** TODO document this */
public:
    // ctors
    Evaluator() : expression(""), cursor(0) {}
    Evaluator(const std::string& input) : expression(input), cursor(0) {}

    int eval();
    /** evaluate the expression already stored */

    int eval(const std::string& input);
    /** store a new expression and evaluate it */
};

#endif // EVALUATOR_H_INCLUDED
