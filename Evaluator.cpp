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
    std::stack<char> digits;

    while (isdigit(expression[cursor]))  // this is safe, returns false, for the end of the string (ISO/IEC 14882:2011 21.4.5)
    {
        digits.push(expression[cursor] - 48);  // convert from ascii
        ++cursor;
    }

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

    return total;
}

void Evaluator::eat_white()
{
    /** moves cursor to next non-whitespace, printable character or end */

    while ((cursor < expression.size()) && (expression[cursor] < FIRST_NON_WHITE))
        ++cursor;
}

std::string Evaluator::cursor_str()
{
    /** returns cursor position for error message */

    std::ostringstream to_return;
    to_return << " @ char: " << cursor;
    return to_return.str();
}

std::queue<Token> Evaluator::make_tokens()
{
    /** tokenizes expression into a queue of operators and operands */

    std::queue<Token> tokens;

    bool next_is_binary = false;

    while (cursor < expression.size())
    {
        eat_white();
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
                tokens.push(Token(expression[cursor]));
                ++cursor;
                break;
            case '>':
                if (expression[cursor+1] == '=')  // >=
                {
                    tokens.push(Token('g'));
                    cursor += 2;
                }
                else  // just >
                {
                    tokens.push(Token('>'));
                    ++cursor;
                }
                break;
            case '<':
                if (expression[cursor+1] == '=')  // <=
                {
                    tokens.push(Token('l'));
                    cursor += 2;
                }
                else  // just <
                {
                    tokens.push(Token('<'));
                    ++cursor;
                }
                break;
            case '=':
            case '&':
            case '|':
                if (expression[cursor+1] != expression[cursor])
                    throw std::invalid_argument("Invalid operator" + cursor_str());
                tokens.push(Token(expression[cursor]));
                cursor += 2;
                break;
            case '!':
                if (expression[cursor+1] != '=')
                    throw std::invalid_argument("Unary operator not allowed" + cursor_str());
                tokens.push(Token('x'));
                cursor += 2;
                break;
                // TODO division by zero locations
            default:
                throw std::invalid_argument("Binary operator required" + cursor_str());
            }
        }
        else // integer or unary
        {
            if (isdigit(expression[cursor]))  // integer
            {
                tokens.push(Token(read_int()));
                next_is_binary = true;
            }
            else  // unary
            {
                switch (expression[cursor])
                {
                case '!':
                    tokens.push(Token('!'));
                    ++cursor;
                    break;
                case '+':
                    if (expression[cursor+1] != '+')
                        throw std::invalid_argument("Binary operator not allowed" + cursor_str());
                    tokens.push(Token('i'));
                    cursor += 2;
                    break;
                case '-':
                    if (expression[cursor+1] == '-')
                    {
                        tokens.push(Token('d'));
                        cursor += 2;
                        break;
                    }
                    // not decrement, has to be negative
                    tokens.push(Token('n'));
                    ++cursor;
                    /* old rule int must follow negative unary
                    eat_white();
                    if (! isdigit(expression[cursor]))
                        throw std::invalid_argument("Unary operator \"-\" must be followed by an integer" + cursor_str());
                    */
                    break;
                    // TODO parentheses
                case '^':
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
                    throw std::invalid_argument("Binary operator not allowed" + cursor_str());
                default:
                    throw std::invalid_argument("Invalid character" + cursor_str());
                }
            }
        }
    }

    return tokens;
}

int Evaluator::eval()
{
    // this is where the magic happens
    /** evaluate the expression already stored */

    int to_return = 0;

    std::queue<Token> tokens = make_tokens();

    // TODO
    for (int i = tokens.size(); i > 0; --i)
    {
        if (tokens.front().is_integer)
            std::cout << tokens.front().integer << ' ';
        else
            std::cout << tokens.front().operat << ' ';
    }


}

int Evaluator::eval(const std::string& input)
{
    /** store a new expression and evaluate it */
    store_exp(input);
    return eval();
}
