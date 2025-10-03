#pragma once
#include <string>
#include <vector>
#include <memory>

using byte = uint8_t;
using Bytes = std::vector<byte>;

using Operands = std::array<byte, 64>;
using Instruction = std::array<byte, 80>;

/**
 * @brief Enumeration for types of tokens in the source code of a script.
 */
enum TokenType
{
    LPAREN,
    RPAREN,
    IDENTIFIER,
    INTEGER,
    FLOAT,
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

// Unique pointer type to use for all ASTNode classes.
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
 * @brief Represents a list (sequence of expressions) in an AST.
 */
class ListNode : public ASTNode
{
public:
    ListNode(std::vector<ASTNodePtr> &&elements)
        : _elements(std::move(elements)) {};

    /**
     * @brief Generates a string representation of the list node.
     *
     * @return The string representation.
     */
    std::string toString() const override;

    /**
     * @brief Gets the underlying vector representation of the list.
     *
     * @return The vector of AST node pointers representing the elements.
     */
    const std::vector<ASTNodePtr> &getElements() const { return _elements; }

private:
    std::vector<ASTNodePtr> _elements;
};

/**
 * @brief Represents an atom (single token) in an AST.
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

    /**
     * @brief Gets the underlying token representation of the atom.
     *
     * @return The token.
     */
    Token getToken() const { return _token; }

private:
    Token _token;
};
