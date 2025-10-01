#pragma once
#include <string>
#include <vector>
#include "types.h"

class Lexer
{
public:
    Lexer(std::string source)
        : _source(source) {};

    std::vector<Token> tokenize();

private:
    std::string _source;
    int _current;
    int _line;
    int _col;

    char peek() const;
    char next();

    void skipWhitespace();
    void skipComment();

    Token makeNumber();
    Token makeIdentifier();
};
