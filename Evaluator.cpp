#include "Evaluator.h"

#include <string>
#include <cctype>
#include <stack>
#include <stdexcept>
#include <queue>

#include "Token.h"

const int Evaluator::FIRST_NON_WHITE = 33;  // first char that prints non-white

void Evaluator::store_exp(const std::string& input)
{
    /** stores string in expression */

    expression = input;
    cursor = 0;
}

int Evaluator::read_int()
{
    /** reads an integer out of the expression from the cursor */

    int to_return = 0;

    while (isdigit(expression[cursor]))  // this is safe, returns false, for the end of the string (ISO/IEC 14882:2011 21.4.5)
    {
        to_return *= 10;
        to_return += (expression[cursor] - '0');  // convert from ascii
        ++cursor;
    }

    return to_return;
}

void Evaluator::eat_white()
{
    /** moves cursor to next non-whitespace, printable character or end */

    while ((cursor < expression.size()) && (expression[cursor] < FIRST_NON_WHITE))
        ++cursor;
}

std::string Evaluator::cursor_str(int where /* = -1 */) const
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
                // simplest binary operators
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
                if (expression[cursor+1] != expression[cursor])  // need 2 in a row
                    throw std::invalid_argument("Invalid operator" + cursor_str());
                tokens.push(Token(expression[cursor], cursor));
                cursor += 2;
                break;
            case '!':
                if (expression[cursor+1] != '=')  // !=
                    throw std::invalid_argument("Unary operator not allowed" + cursor_str());
                tokens.push(Token('x', cursor));
                cursor += 2;
                break;
            case ')':
                next_is_binary = true;  // still need another binary after this
                tokens.push(Token(')', cursor));
                ++cursor;
                break;
            default:
                throw std::invalid_argument("Binary operator required" + cursor_str());
            }
        }
        else  // integer or unary
        {
            if (isdigit(expression[cursor]))  // integer
            {
                tokens.push(Token(read_int(), cursor));
                next_is_binary = true;
            }
            else  // unary
            {
                switch (expression[cursor])
                {
                case '(':
                case '!':
                    tokens.push(Token(expression[cursor], cursor));
                    ++cursor;
                    break;
                case '+':
                    if (expression[cursor+1] != '+')
                        throw std::invalid_argument("Binary operator not allowed" + cursor_str());
                    tokens.push(Token('i', cursor));  // increment
                    cursor += 2;
                    break;
                case '-':
                    if (expression[cursor+1] == '-')
                    {
                        tokens.push(Token('d', cursor));  // decrement
                        cursor += 2;
                        break;
                    }
                    // not decrement, has to be negative
                    tokens.push(Token('n', cursor));  // negative
                    ++cursor;
                    break;
                default:
                    if (tokens.empty())
                        throw std::invalid_argument("Must begin with integer or unary operator or open parenthesis" + cursor_str());
                    throw std::invalid_argument("Integer or unary operator or open parenthesis required" + cursor_str());
                }
            }
        }

        eat_white();
    }

    if (! next_is_binary)  // expression has to end where a binary operator would come next
    {
        if (tokens.empty())
            throw std::invalid_argument("Empty expression" + cursor_str());
        throw std::invalid_argument("Expression must end with integer (or close parenthesis)" + cursor_str());
    }

    return tokens;
}

// the algorithm of this function is inspired by Dr. Kuhail's examples, adapted and modified
void Evaluator::process_operator(std::stack<int>& operands, std::stack<Token>& operators, const Token& operator_) const
{
    /** handles order of operations for this operator
        compared to other operators on the operator stack */

    if (operators.empty() || (operator_.operat == '('))
    {
        if (operator_.operat == ')')
            throw std::invalid_argument("Unmatched close parenthesis" + cursor_str(operator_.location));
        operators.push(operator_);
    }
    else  // stack not empty and not open parenthesis
    {
        if ((operator_ > operators.top()) || (operator_.is_unary))  // all unary > other unary (even though same precedence #)
            operators.push(operator_);
        else  // operator_ has lower (or equal) precedence than top
        {
            // evaluate all stacked operators with equal
            // or higher precedence than operator_
            while (! operators.empty() && (operators.top().operat != '(') && (operator_ <= operators.top()))
            {
                // evaluate top operator and put result on operand stack
                operands.push(evaluate_one_operator(operands, operators.top()));
                operators.pop();
            }
            // assert: operator stack empty or
            //         top of stack is '(' or
            //         operator_ precedence > top of stack
            if (operator_.operat == ')')
            {
                if (!operators.empty() && (operators.top().operat == '('))
                    operators.pop();
                else  // stack empty
                    throw std::invalid_argument("Unmatched close parenthesis" + cursor_str(operator_.location));
            }
            else  // operator_ not parentheses and greater precedence than top
                operators.push(operator_);
        }
    }
}

int Evaluator::eval_tokens(std::queue<Token>& tokens) const
{
    /** evaluate the expression from queue of tokens */

    std::stack<int> operands;
    std::stack<Token> operators;

    while (! tokens.empty())
    {
        if (tokens.front().is_integer)
            operands.push(tokens.front().integer);
        else  // is operator
            process_operator(operands, operators, tokens.front());
        tokens.pop();
    }
    // assert: operator stack has no close parentheses,
    //         if stack has open parentheses, it is invalid expression
    //         (evaluate_one_operator will handle this)
    //         else: only binary and unary operators
    //         in order of precedence
    //         highest at top of stack

    while (! operators.empty())
    {
        operands.push(evaluate_one_operator(operands, operators.top()));
        operators.pop();
    }
    // assert: operands has only 1 integer
    //         (because the validity of the expression was checked
    //          when making the tokens and processing operators)

    return operands.top();
}

int Evaluator::evaluate_one_operator(std::stack<int>& operands, const Token& operator_) const
{
    /** evaluates the operator with operands from the operand stack
        places the result on the operand stack */

    if (operator_.operat == '(')
        throw std::invalid_argument("Unmatched open parenthesis" + cursor_str(operator_.location));

    int lhs, rhs, result;

    // pull needed operands
    rhs = operands.top();
    operands.pop();
    if (! operator_.is_unary)
    {
        lhs = operands.top();
        operands.pop();
    }

    // evaluate
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
    /** translate the stored expression into tokens
        and evaluate the tokens */

    cursor = 0;

    std::queue<Token> tokens = make_tokens();

    return eval_tokens(tokens);
}

int Evaluator::eval(const std::string& input)
{
    /** store a new expression and evaluate it */

    store_exp(input);
    return eval();
}
