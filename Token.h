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
    bool is_unary;

    void check_unary();
    /** assigns value to is_unary */

    bool operator> (const Token& rhs) const;
    /** compares integers, or if not integer, compares precedence
        @throws std::invalid_argument for compare integer and non-integer */
    bool operator< (const Token& rhs) const { return rhs.operator> (*this); }
    bool operator<= (const Token& rhs) const { return (! operator> (rhs)); }
    bool operator>= (const Token& rhs) const { return (! rhs.operator> (*this)); }

    // ctors
    Token() : integer(0), operat(), location(0), is_integer(true), is_unary(false) {}
    Token(const int& i, const int& cursor) : integer(i), operat(), location(cursor), is_integer(true), is_unary(false) {}
    Token(const char& o, const int& cursor) : integer(), operat(o), location(cursor), is_integer(false) { check_unary(); }
};

#endif // TOKEN_H_INCLUDED
