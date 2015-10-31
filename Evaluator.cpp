#include "Evaluator.h"

#include <string>
#include <cctype>
#include <stack>
#include <iostream>  // TODO this was just for test, get rid of it later
#include <stdexcept>
#include <queue>
#include <sstream>

#include "Token.h"

const int Evaluator::FIRST_NON_WHITE = 33;  // first char that prints non-white
// const int Evaluator::LAST_NON_WHITE = 126;  // last (standard) char that prints non-white

void Evaluator::store_exp(const std::string& input)
{
    /** stores string in expression */
    expression = input;
    cursor = 0;
}

void Evaluator::reset_exp()
{
    /** resets the cursor */
    cursor = 0;
}

int Evaluator::read_int()
{
    /** reads an integer out of the expression from the cursor */

    // make stack of digits
    // std::stack<char> digits;
    int to_return = 0;

    while (isdigit(expression[cursor]))  // this is safe, returns false, for the end of the string (ISO/IEC 14882:2011 21.4.5)
    {
        to_return *= 10;
        to_return += (expression[cursor] - '0');  // convert from ascii
        ++cursor;
    }

    // TODO clean up
    /*
    // add up stack of digits
    int total = 0;
    int exponent = 0;  // 10 ^ exponent
    int this_digit;

    while (! digits.empty())
    {
        this_digit = digits.top();
        for (int i = exponent; i > 0; --i)
            this_digit *= 10;
        total += this_digit;

        ++exponent;
        digits.pop();
    }
    */

    return to_return;
}

void Evaluator::eat_white()
{
    /** moves cursor to next non-whitespace, printable character or end */

    while ((cursor < expression.size()) && (expression[cursor] < FIRST_NON_WHITE))
        ++cursor;
}

std::string Evaluator::cursor_str(int where /*= -1*/) const
{
    /** returns a cursor position for error message */

    if (where == -1)
        where = cursor;

    return " @ char: " + std::to_string(where);
}

std::queue<Token> Evaluator::make_tokens()
{
    /** tokenizes expression into a queue of operators and operands */

    std::queue<Token> tokens;

    bool next_is_binary = false;

    eat_white();
    while (cursor < expression.size())
    {
        if (next_is_binary)
        {
            next_is_binary = false;

            switch (expression[cursor])
            {
            case '^':
            case '*':
            case '/':
            case '%':
            case '+':
            case '-':
                tokens.push(Token(expression[cursor], cursor));
                ++cursor;
                break;
            case '>':
                if (expression[cursor+1] == '=')  // >=
                {
                    tokens.push(Token('g', cursor));
                    cursor += 2;
                }
                else  // just >
                {
                    tokens.push(Token('>', cursor));
                    ++cursor;
                }
                break;
            case '<':
                if (expression[cursor+1] == '=')  // <=
                {
                    tokens.push(Token('l', cursor));
                    cursor += 2;
                }
                else  // just <
                {
                    tokens.push(Token('<', cursor));
                    ++cursor;
                }
                break;
            case '=':
            case '&':
            case '|':
                if (expression[cursor+1] != expression[cursor])
                    throw std::invalid_argument("Invalid operator" + cursor_str());
                tokens.push(Token(expression[cursor], cursor));
                cursor += 2;
                break;
            case '!':
                if (expression[cursor+1] != '=')
                    throw std::invalid_argument("Unary operator not allowed" + cursor_str());
                tokens.push(Token('x', cursor));
                cursor += 2;
                break;
            case ')':
                next_is_binary = true;
                tokens.push(Token(')', cursor));
                ++cursor;
                break;
            default:
                throw std::invalid_argument("Binary operator required" + cursor_str());
            }
        }
        else // integer or unary
        {
            if (isdigit(expression[cursor]))  // integer
            {
                int temp = cursor;  // need the value before it changes  // TODO do I?
                tokens.push(Token(read_int(), temp));
                next_is_binary = true;
            }
            else  // unary
            {
                switch (expression[cursor])
                {
                case '!':
                    tokens.push(Token('!', cursor));
                    ++cursor;
                    break;
                case '+':
                    if (expression[cursor+1] != '+')
                        throw std::invalid_argument("Binary operator not allowed" + cursor_str());
                    tokens.push(Token('i', cursor));
                    cursor += 2;
                    break;
                case '-':
                    if (expression[cursor+1] == '-')
                    {
                        tokens.push(Token('d', cursor));
                        cursor += 2;
                        break;
                    }
                    // not decrement, has to be negative
                    tokens.push(Token('n', cursor));
                    ++cursor;
                    /* old rule int must follow negative unary
                    eat_white();
                    if (! isdigit(expression[cursor]))
                        throw std::invalid_argument("Unary operator \"-\" must be followed by an integer" + cursor_str());
                    */
                    break;
                case '(':
                    tokens.push(Token('(', cursor));
                    ++cursor;
                    break;
                /*case '^':
                case '*':
                case '/':
                case '%':
                case '>':
                case '<':
                case '=':
                case '&':
                case '|':
                    if (tokens.empty())
                        throw std::invalid_argument("Expression can't start with a binary operator" + cursor_str());
                    if (! (tokens.back().is_unary() || tokens.back().is_integer))
                        throw std::invalid_argument("Two binary operators in a row" + cursor_str());
                    throw std::invalid_argument("Binary operator not allowed" + cursor_str());*/
                default:
                    if (tokens.empty())
                        throw std::invalid_argument("Must begin with integer or unary operator" + cursor_str());
                    throw std::invalid_argument("Integer or unary operator required" + cursor_str());
                }
            }
        }

        eat_white();
    }

    if (! next_is_binary)
    {
        throw std::invalid_argument("Expression must end with integer" + cursor_str());
    }

    return tokens;
}

// the algorithm of this function is adapted from Dr. Kuhail's examples and modified
void Evaluator::process_operator(std::stack<int>& operands, std::stack<Token>& operators, const Token& operator_) const
{
    if (operators.empty() || (operator_.operat == '(') || (operator_.operat == '[') || (operator_.operat == '{'))
    {
        if (operator_.operat == ')' || operator_.operat == ']' || operator_.operat == '}')
            throw std::invalid_argument("Unmatched close parenthesis" + cursor_str(operator_.location));
        operators.push(operator_);
    }
    else  // stack not empty and not open parenthesis
    {
        if ((operator_ > operators.top()) || (operator_.is_unary()))
        {
            operators.push(operator_);
        }
        else  // op has lower precedence than top
        {
            // Pop all stacked operators with equal
            // or higher precedence than operator_.
            while (! operators.empty()
                   && (operators.top().operat != '(')
                   && (operators.top().operat != '[')
                   && (operators.top().operat != '{')
                   && (operator_ <= operators.top()))
            {
                operands.push(evaluate_one_operator(operands, operators.top()));
                // evaluate top operator
                // put result on operand stack
                // TODO division by zero check and error
                //postfix += operator_stack.top();  // TODO clean up
                //postfix += " ";
                operators.pop();
            }
            // assert: Operator stack is empty or
            //         top of stack is '(' or current
            //         operator precedence > top of stack operator
            //         precedence;
            if (operator_.operat == ')')
            {
                if (!operators.empty() && (operators.top().operat == '('))
                {
                    operators.pop();
                }
                else  // stack empty or different parentheses
                {
                    throw std::invalid_argument("Unmatched close parenthesis" + cursor_str(operator_.location));
                }
            }
            else if (operator_.operat == ']')
            {
                if (!operators.empty() && (operators.top().operat == '['))
                {
                    operators.pop();
                }
                else  // stack empty or different parentheses
                {
                    throw std::invalid_argument("Unmatched close parenthesis" + cursor_str(operator_.location));
                }
            }
            else if (operator_.operat == '}')
            {
                if (!operators.empty() && (operators.top().operat == '{'))
                {
                    operators.pop();
                }
                else  // stack empty or different parentheses
                {
                    throw std::invalid_argument("Unmatched close parenthesis" + cursor_str(operator_.location));
                }
            }
            else  // op not parentheses and greater precedence than top
            {
                operators.push(operator_);
            }
        }
    }
}

int Evaluator::eval_tokens(std::queue<Token>& tokens) const
{
    /** evaluate the expression from quque of tokens */

    std::stack<int> operands;
    std::stack<Token> operators;

    while (! tokens.empty())
    {
        if (tokens.front().is_integer)
        {
            operands.push(tokens.front().integer);
        }
        else  // is operator
        {
            process_operator(operands, operators, tokens.front());
        }
        tokens.pop();
    }
    // assert: operator stack has no parentheses,
    //         only binary and unary operators
    //         in order of precedence
    //         highest at top of stack

    while (! operators.empty())
    {
        operands.push(evaluate_one_operator(operands, operators.top()));
        operators.pop();
    }

    /* TODO get rid of this test when I don't need it:
    for (int i = tokens.size(); i > 0; --i)
    {
        if (tokens.front().is_integer)
            std::cout << tokens.front().integer << ' ';
        else
            std::cout << tokens.front().operat << ' ';
        tokens.pop();
    }*/

    // TODO get rid of this too
    // std::cout << operands.size() << std::endl << operators.size() << std::endl << operands.top() << std::endl << operators.top().operat << std::endl;

    return operands.top();
}

int Evaluator::evaluate_one_operator(std::stack<int>& operands, const Token& operator_) const
{
    int lhs, rhs, result;

    rhs = operands.top();
    operands.pop();
    if (! operator_.is_unary())
    {
        lhs = operands.top();
        operands.pop();
    }

    switch (operator_.operat)
    {
    case '!':
        return (! rhs);
    case 'i':
        return rhs + 1;
    case 'd':
        return rhs - 1;
    case 'n':
        return rhs * (-1);
    case '^':
        if ((lhs == 0) && (rhs == 0))
            throw std::invalid_argument("Undefined expression 0^0" + cursor_str(operator_.location));
        if ((lhs == 0) || (lhs == 1))  // 0 ^ rhs   1 ^ rhs   not 0^0
            return lhs;
        if (lhs == -1)  // -1 ^ rhs
        {
            if (rhs % 2)  // odd
                return -1;
            else  // even
                return 1;
        }
        if (rhs < 0)  // negative exponent with lhs != -1,0,1
            throw std::invalid_argument("Non-integer expression (negative exponent)" + cursor_str(operator_.location));
        result = 1;
        for (int i = rhs; i > 0; --i)
            result *= lhs;
        return result;
    case '*':
        return lhs * rhs;
    case '/':
        if (rhs == 0)
            throw std::invalid_argument("Division by zero" + cursor_str(operator_.location));
        return lhs / rhs;
    case '%':
        return lhs % rhs;
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    case '>':
        return (lhs > rhs);
    case 'g':
        return (lhs >= rhs);
    case '<':
        return (lhs < rhs);
    case 'l':
        return (lhs <= rhs);
    case '=':
        return (lhs == rhs);
    case 'x':
        return (lhs != rhs);
    case '&':
        return (lhs && rhs);
    case '|':
        return (lhs || rhs);
    default:
        throw std::invalid_argument("How did I get here? This shouldn't be possible!" + cursor_str(operator_.location));
    }
}

int Evaluator::eval()
{
    // this is where the magic happens
    /** evaluate the expression already stored */

    int to_return = 0;

    std::queue<Token> tokens = make_tokens();

    return eval_tokens(tokens);
}

int Evaluator::eval(const std::string& input)
{
    /** store a new expression and evaluate it */
    store_exp(input);
    return eval();
}
