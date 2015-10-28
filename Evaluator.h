#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

#include <string>
#include <sstream>

class Evaluator
{
private:
    std::istringstream expression;

    void store_exp(const std::string& input);
    /** stores string in expression */
    void reset_exp();
    /** resets the stringstream */
public:
    Evaluator() : expression("") {}
    Evaluator(const std::string& input) : expression(input) {}

    int eval();
    /** evaluate the expression already stored */

    int eval(const std::string& input);
    /** store a new expression and evaluate it */
};

#endif // EVALUATOR_H_INCLUDED
