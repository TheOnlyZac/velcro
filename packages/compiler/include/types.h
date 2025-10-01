#pragma once
#include <string>
#include <vector>
#include <memory>

enum TokenType
{
    LPAREN, RPAREN,
    IDENTIFIER,
    INTEGER, FLOAT,
    COMMENT,
    EOF_TOKEN
};

struct Token
{
    TokenType type;
    std::string image;
    int line;
    int col;
};

class ASTNode;
using ASTNodePtr = std::unique_ptr<ASTNode>;

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
};

class ListNode : ASTNode
{
public:
    std::string toString() const override
    {
        std::string result = "(";
        for (const ASTNodePtr &elem : _elements)
        {
            result += elem->toString() + " ";
        }
        result += ")";
        return result;
    }
private:
    std::vector<ASTNodePtr> _elements;
};

class AtomNode : ASTNode
{
public:
    AtomNode(Token token)
        : _token(token) {};

    std::string toString() const override
    {
        return _token.image;
    }

private:
    Token _token;
};
