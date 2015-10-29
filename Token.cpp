#include "Token.h"

#include <string>

const std::string Token::UNARY_OPERATORS = "!idn";

const std::string Token::BINARY_OPERATORS = "^*/%+->g<l=x&|()[]{}";
const int Token::PRECEDENCE[] = { 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 1, -1, -1, -1, -1, -1, -1 };

bool Token::is_unary()
{
    for (int i = 0; i < UNARY_OPERATORS.size(); ++i)
        if (operat == UNARY_OPERATORS[i])
            return true;
    return false;
}

