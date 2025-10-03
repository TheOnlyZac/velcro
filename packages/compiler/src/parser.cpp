#include "parser.h"

ASTNodePtr Parser::parse()
{
    _current = 0;

    std::vector<ASTNodePtr> expressions;
    while (!isAtEnd())
    {
        ASTNodePtr expr = parseExpression();
        if (expr)
            expressions.push_back(std::move(expr));
    }

    if (expressions.empty())
        return nullptr;

    if (expressions.size() == 1)
        return std::move(expressions[0]);

    // If multiple expressions, wrap them in a program node (list)
    return std::make_unique<ListNode>(std::move(expressions));
}

bool Parser::isAtEnd() const
{
    return _current >= _tokens.size() || _tokens[_current].type == EOF_TOKEN;
}

Token *Parser::peek() const
{
    if (isAtEnd())
        return nullptr;

    return const_cast<Token*>(&_tokens[_current]);
}

Token *Parser::advance()
{
    if (isAtEnd())
        return nullptr;

    return const_cast<Token*>(&_tokens[_current++]);
}

ASTNodePtr Parser::parseExpression()
{
    Token *t = peek();
    if (t == nullptr)
        return nullptr;

    if (t->type == LPAREN)
        return parseList();

    return parseAtom();
}

ASTNodePtr Parser::parseList()
{
    Token *t = advance();
    if (t == nullptr)
        return nullptr;

    std::vector<ASTNodePtr> elements;
    Token *next = peek();
    while (next != nullptr && next->type != RPAREN)
    {
        ASTNodePtr expr = parseExpression();
        if (expr)
            elements.push_back(std::move(expr));
        next = peek();
    }

    // Consume the closing RPAREN
    if (next != nullptr && next->type == RPAREN)
        advance();

    return std::make_unique<ListNode>(std::move(elements));
}

ASTNodePtr Parser::parseAtom()
{
    Token *t = advance();
    if (t == nullptr)
        return nullptr;

    return std::make_unique<AtomNode>(*t);
}
