#ifndef  __TOKENIZER__H__
#define __TOKENIZER__H__

#include <string>

enum TokenType
{
    EOFLINE,
    Add,
    Minus,
    Multiply,
    Power,
    Divide,
    Modulus,
    OpenParens,
    CloseParens,
    DoubleNumber,
    IntNumber,
    HexIntNumber,
    TernaryQuestion,
    TernaryDivider,
    Greater,
    Less,
    GreaterOrEqual,
    LessOrEqual,
    Equal,
    NotEqual
};

class Tokenizer
{
public:
    TokenType tokenType;
    double number = 0.0;
    long longNumber = 0;

    Tokenizer(std::string expression_);
    Tokenizer(std::string expression_, char tmplate, long templateValue);

    void NextToken();

private:

    char currentChar;
    char templateSymbol = '\0';
    long templateValue = 0;

    void GoToNextChar();

    long GetHexNumber();

    char CheckNextChar();
    bool IsHex(char symbol);
    std::string GetErrorMessage();

    std::string expression;
    unsigned int pos = 0;
};


#endif