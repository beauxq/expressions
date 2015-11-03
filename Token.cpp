#include "Token.h"

#include <string>
#include <stdexcept>

const std::string Token::UNARY_OPERATORS = "!idn";

const std::string Token::BINARY_OPERATORS = "^*/%+->g<l=x&|()";
const int Token::PRECEDENCE[] = { 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 1, -1, -1 };

bool Token::is_unary() const
{
    for (int i = 0; i < UNARY_OPERATORS.size(); ++i)
        if (operat == UNARY_OPERATORS[i])
            return true;
    return false;
}

bool Token::operator> (const Token& rhs) const
{
    /** compares integers, or if not integer, compares precedence
        @throws std::invalid_argument for compare integer and non-integer */

    if (is_integer != rhs.is_integer)
        throw std::invalid_argument("compare integer token with non-integer token");

    if (is_integer)
        return integer > rhs.integer;

    int this_precedence, rhs_precedence;

    if (is_unary())
        this_precedence = 8;
    else  // this not unary
        this_precedence = PRECEDENCE[BINARY_OPERATORS.find(operat)];

    if (rhs.is_unary())
        rhs_precedence = 8;
    else  // rhs not unary
        rhs_precedence = PRECEDENCE[BINARY_OPERATORS.find(rhs.operat)];

    return this_precedence > rhs_precedence;
}
