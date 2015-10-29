#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>

struct Token
{
    static const std::string UNARY_OPERATORS;

    static const std::string BINARY_OPERATORS;
    static const int PRECEDENCE[];

    const int integer;
    const char operat;
    bool is_integer;

    bool is_unary();

    // ctors
    Token() : integer(0), operat(), is_integer(true) {}  // just to make a valid state - this should never be used
    Token(const int& x) : integer(x), operat(), is_integer(true) {}
    Token(const char& c) : integer(), operat(c), is_integer(false) {}
};

#endif // TOKEN_H_INCLUDED
