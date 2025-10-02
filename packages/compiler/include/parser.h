#pragma once
#include <vector>
#include "types.h"

class Parser
{
public:
    Parser(std::vector<Token> tokens)
        : _tokens(tokens) {};

    ASTNodePtr parse();

private:
    std::vector<Token> _tokens;
    int _current;

    bool isAtEnd() const;

    Token *peek() const;
    Token *advance();

    ASTNodePtr parseExpression();
    ASTNodePtr parseList();
    ASTNodePtr parseAtom();
};
