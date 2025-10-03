#pragma once
#include <vector>
#include "types.h"

class Parser
{
public:
    Parser(std::vector<Token> tokens)
        : _tokens(tokens) {};

    /**
     * @brief Converts the list of tokens into an Abstract Syntax Tree (AST).
     *
     * @return The root node of the AST.
     */
    ASTNodePtr parse();

private:
    std::vector<Token> _tokens;
    int _current;

    /**
     * @brief Checks whether the current token is the last token.
     *
     * @returns True if at the end, otherwise false.
     */
    bool isAtEnd() const;

    /**
     * @brief Reads the next token without advancing the current position.
     *
     * @return The next token.
     */
    Token *peek() const;

    /**
     * @brief Reads the next token and advances the current position.
     *
     * @return The next token.
     */
    Token *advance();

    /**
     * @brief Parses an expression, which can be either an atom or a list.
     *
     * @return The parsed AST node.
     */
    ASTNodePtr parseExpression();

    /**
     * @brief Parses a list, which is a sequence of expressions enclosed in
     * parentheses.
     *
     * @return The parsed ListNode AST node.
     */
    ASTNodePtr parseList();

    /**
     * @brief Parses an atom, which is a single token.
     *
     * @return The parsed AtomNode AST node.
     */
    ASTNodePtr parseAtom();
};
