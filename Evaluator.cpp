#include "Evaluator.h"

#include <string>

void Evaluator::store_exp(const std::string& input)
{
    /** stores string in expression */
    expression.str(input);
    expression.clear();
}

void Evaluator::reset_exp()
{
    /** resets the stringstream */
    expression.clear();
    expression.str(expression.str());
}

int Evaluator::eval()
{
    /** evaluate the expression already stored */
}

int Evaluator::eval(const std::string& input)
{
    /** store a new expression and evaluate it */

}
