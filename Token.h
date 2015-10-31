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
    const int location;  // location of this token in the string
    const bool is_integer;

    bool is_unary() const;

    bool operator> (const Token& rhs) const;
    /** compares integers, or if not integer, compares precedence
        @throws std::invalid_argument for compare integer and non-integer */
    bool operator< (const Token& rhs) const { return rhs.operator> (*this); }
    bool operator<= (const Token& rhs) const { return (! operator> (rhs)); }
    bool operator>= (const Token& rhs) const { return (! rhs.operator> (*this)); }

    // ctors
    Token() : integer(0), operat(), location(0), is_integer(true) {}  // just to make a valid state - this should never be used
    Token(const int& x, const int& cursor) : integer(x), operat(), location(cursor), is_integer(true) {}
    Token(const char& c, const int& cursor) : integer(), operat(c), location(cursor), is_integer(false) {}
};

#endif // TOKEN_H_INCLUDED
