#pragma once
#include <string>
#include <vector>
#include "types.h"

class Lexer
{
public:
    Lexer(std::string source)
        : _source(source) {};

    /**
     * @brief Converts the source text into tokens.
     *
     * @return The vector of parsed tokens.
     */
    std::vector<Token> tokenize();

private:
    std::string _source;
    size_t _current = 0;
    int _line = 1;
    int _col = 1;

    /**
     * @brief Checks whether the current character is the last character.
     *
     * @returns True if at the end, otherwise false.
     */
    bool isAtEnd() const;

    /**
     * @brief Reads the next character of the source text without advancing
     * the current ?.
     *
     * @return The next character in the source text.
     */
    char peek() const;

    /**
     * @brief Reads the next character of the source text and advances the
     * current position.
     *
     * @return The next character of the source text.
     */
    char next();

    /**
     * @brief Reads characters until it reaches a non-whitespace character.
     */
    void skipWhitespace();

    /**
     * @brief Looks for the start of a comment and then reads characters until
     * it finds the end of the comment.
     */
    void skipComment();

    /**
     * @brief Reads characters to create either an integer or float.
     *
     * @return The number token.
     */
    Token makeNumber();

    /**
     * @brief Reads characters to create an identifier.
     *
     * @return The identifier token.
     */
    Token makeIdentifier();
};
