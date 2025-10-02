#include "lexer.h"
#include <cctype>

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (!isAtEnd())
    {
        char c = peek();
        switch (c)
        {
            case '(':
                tokens.push_back(Token{LPAREN, "(", _line, _col});
                next();
                break;
            case ')':
                tokens.push_back(Token{RPAREN, ")", _line, _col});
                next();
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                tokens.push_back(makeNumber());
                break;
            }
            case ';':
                skipComment();
                break;
            default:
                if (std::isspace(static_cast<unsigned char>(c)))
                {
                    skipWhitespace();
                    break;
                }
                tokens.push_back(makeIdentifier());
                break;
        }
    }

    return tokens;
}

bool Lexer::isAtEnd() const
{
    return _current >= _source.size();
}

char Lexer::peek() const
{
    return isAtEnd() ? '\0' : _source[_current];
}

char Lexer::next()
{
    if (isAtEnd())
        return '\0';

    char c = _source[_current++];
    if (c == '\n')
    {
        _line++;
        _col = 1;
    }
    else
    {
        _col++;
    }

    return c;
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd())
    {
        char c = peek();
        if (std::isspace(static_cast<unsigned char>(c)))
        {
            next();
        }
        else
        {
            break;
        }
    }
}

void Lexer::skipComment()
{
    if (peek() != ';')
        return;

    while (!isAtEnd())
    {
        char c = peek();
        if (c == '\n')
        {
            break;
        }
        else
        {
            next();
        }
    }
}

Token Lexer::makeNumber()
{
    int col = _col;
    int line = _line;
    std::string image;
    bool isFloat = false;

    while (!isAtEnd())
    {
        char c = peek();
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            image.push_back(c);
            next();
        }
        else if (c == '.' && !isFloat)
        {
            isFloat = true;
            image.push_back(c);
            next();
        }
        else
        {
            break;
        }
    }

    return Token{isFloat ? FLOAT : INTEGER, image, line, col};
}

Token Lexer::makeIdentifier()
{
    int col = _col;
    int line = _line;
    std::string image;

    while (!isAtEnd())
    {
        char c = peek();
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_')
        {
            image.push_back(c);
            next();
        }
        else
        {
            break;
        }
    }

    return Token{IDENTIFIER, image, line, col};
}
