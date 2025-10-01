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

    Token *peek() const;
    Token *advance() const;

    bool isAtEnd() const;

    ASTNodePtr parseExpression();
    ASTNodePtr parseList();
    ASTNodePtr parseAtom();
};
