#include "Tokenizer.h"
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <cstring>

bool isWhiteSpace(char s);

Tokenizer::Tokenizer(std::string expression_)
{
    expression = expression_;
    currentChar = expression.at(0);
    NextToken();
}

Tokenizer::Tokenizer(std::string expression_, char tmplate, long templateValue_)
{
    expression = expression_;

    templateSymbol = tmplate;
    templateValue = templateValue_;

    currentChar = expression.at(0);
    NextToken();
}


void Tokenizer::NextToken()
{
    while (isWhiteSpace(currentChar))
    {
        GoToNextChar();
    }

    switch (currentChar)
    {
    case '\0':
        tokenType = TokenType::EOFLINE;
        return;

    case '+':
        GoToNextChar();
        tokenType = TokenType::Add;
        return;

    case '-':
        GoToNextChar();
        tokenType = TokenType::Minus;
        return;

    case '*':
        GoToNextChar();
        if (currentChar == '*')
        {
            GoToNextChar();
            tokenType = TokenType::Power;
        }
        else
        {
            tokenType = TokenType::Multiply;
        }
        return;

    case '/':
        GoToNextChar();
        tokenType = TokenType::Divide;
        return;

    case '%':
        GoToNextChar();
        if (currentChar == templateSymbol)
        {
            GoToNextChar();
            tokenType = TokenType::DoubleNumber;
            number = templateValue;
        }
        else
        {
            tokenType = TokenType::Modulus;
        }
        return;

    case '(':
        GoToNextChar();
        tokenType = TokenType::OpenParens;
        return;

    case ')':
        GoToNextChar();
        tokenType = TokenType::CloseParens;
        return;

    case '>':
        GoToNextChar();
        if (currentChar == '=')
        {
            GoToNextChar();
            tokenType = TokenType::GreaterOrEqual;
        }
        else
        {
            tokenType = TokenType::Greater;
        }
        return;

    case '<':
        GoToNextChar();
        if (currentChar == '=')
        {
            GoToNextChar();
            tokenType = TokenType::LessOrEqual;
        }
        else
        {
            tokenType = TokenType::Less;
        }
        return;

    case '?':
        GoToNextChar();
        tokenType = TokenType::TernaryQuestion;
        return;

    case ':':
        GoToNextChar();
        tokenType = TokenType::TernaryDivider;
        return;

    case ',':
        GoToNextChar();
        tokenType = TokenType::Comma;
        return;

    case '=':
        GoToNextChar();
        if (currentChar == '=')
        {
            GoToNextChar();
            tokenType = TokenType::Equal;
        }
        else   // only '==' is valid
        {
            throw new std::invalid_argument(GetErrorMessage());
        }
        return;

    case '!':
        GoToNextChar();
        if (currentChar == '=')
        {
            GoToNextChar();
            tokenType = TokenType::NotEqual;
        }
        else
        {
            throw new std::invalid_argument(GetErrorMessage());
        }
        return;
    }

    // Any numeric literal
    if (isdigit(currentChar) || currentChar == '.')
    {
        // hex literals
        if (currentChar == '0')
        {
            char hexLiteral = CheckNextChar();
            if ((char)hexLiteral == 'x' || (char)hexLiteral == 'X')
            {
                GoToNextChar();  // Skip 0x
                GoToNextChar();
                number = GetHexNumber();
                tokenType = TokenType::DoubleNumber;
                return;
            }
        }

        // Double or integer literal
        std::string sb = "";
        bool dotSymbol = false;
        while (isdigit(currentChar) || (!dotSymbol && currentChar == '.'))
        {
            sb += currentChar;
            if(!dotSymbol)
                dotSymbol = currentChar == '.';
            GoToNextChar();
        }

        // All numeric literals are double
        tokenType = TokenType::DoubleNumber;

        number = atof(sb.c_str());
        return;
    }

    // function names without commas, starts from letter
    if (isalpha(currentChar))
    {
        stringValue = "";
        while (isalnum(currentChar) || currentChar == '.')
        {
            stringValue += currentChar;
            GoToNextChar();
        }
        tokenType = TokenType::Function;
        return;
    }

    throw new std::invalid_argument(GetErrorMessage());
}

void Tokenizer::GoToNextChar()
{
    unsigned int nextPos = pos + 1;
    if (nextPos >= expression.size())
        currentChar = 0;
    else
        currentChar = expression.at(++pos);
}

char Tokenizer::CheckNextChar()
{
    if (pos + 1 >= expression.size())
        return 0;

    return expression.at(pos + 1);
}

long Tokenizer::GetHexNumber()
{
    std::string str = "";
    while (IsHex(currentChar))
    {
        str.append(1, currentChar);
        GoToNextChar();
    }

    // dot symbol in hex literal is syntax error
    if (currentChar == '.')
    {
        throw new std::invalid_argument(GetErrorMessage());
    }

    return std::stoul(str, nullptr, 16);
}

bool Tokenizer::IsHex(char symbol)
{
    return isdigit(symbol) ||
        symbol >= 'a' && symbol <= 'f' || symbol >= 'A' && symbol <= 'F';
}

std::string Tokenizer::GetErrorMessage()
{
    std::string s = "Unexpected character: ";
    s.append(1, currentChar);
    return s;
}

bool isWhiteSpace(char symbol)
{
    if (symbol == ' ')
        return true;
    else
        return false;
}
