#pragma once
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Enumeration for types of tokens in the source code of a script.
 */
enum TokenType
{
    LPAREN, RPAREN,
    IDENTIFIER,
    INTEGER, FLOAT,
    COMMENT,
    EOF_TOKEN
};

/**
 * @brief Represents a single token in the source code of a script.
 */
struct Token
{
    TokenType type;
    std::string image;
    int line;
    int col;
};

// Declare ASTNode unique pointer type to share between ASTNode derived classes.
class ASTNode;
using ASTNodePtr = std::unique_ptr<ASTNode>;

/**
 * @brief Represents a node in an AST for a Velcro program.
 */
class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual std::string toString() const = 0;
};

/**
 * @brief Represents a list node in an AST.
 */
class ListNode : public ASTNode
{
public:
    ListNode(std::vector<ASTNodePtr>&& elements)
        : _elements(std::move(elements)) {};

    /**
     * @brief Generates a string representation of the list node.
     *
     * @return The string representation.
     */
    std::string toString() const override;

private:
    std::vector<ASTNodePtr> _elements;
};

/**
 * @brief Represents an atom node in an AST.
 */
class AtomNode : public ASTNode
{
public:
    AtomNode(Token token)
        : _token(token) {};

    /**
     * @brief Generates a string representation of the atom node.
     *
     * @return The string representation.
     */
    std::string toString() const override;

private:
    Token _token;
};
