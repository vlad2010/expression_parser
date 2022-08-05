#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <math.h>

#include "Parser.h"
#include "Tokenizer.h"
#include "Functions.h"

Parser::Parser(std::string& expr)
{
    tokenizer = std::make_unique<Tokenizer>(expr);
}

Parser::Parser(std::string& expr, char templateSymbol, long templateValue)
{
    tokenizer = std::make_unique<Tokenizer>(expr, templateSymbol, templateValue);
}

Value Parser::ParseExpression()
{
    Value expressionValue;

    // Start from lowest priority operations
    ParseTernaryOperator(expressionValue);

    if (tokenizer->tokenType != TokenType::EOFLINE)
    {
        throw new std::invalid_argument("Unexpected characters at end of expression");
    }

    return expressionValue;
}


void Parser::ParseTernaryOperator(Value& value)
{
    // Parse the left hand side
    ParseComparison(value);

    if (tokenizer->tokenType == TokenType::TernaryQuestion)
    {
        if (value.type != ValueType::BOOL)
        {
            // any numeric value means True for Ternary operator.
            value.Assign(true);
        }
        Value positiveBranch;
        Value negativeBranch;

        tokenizer->NextToken();
        ParseTernaryOperator(positiveBranch);
        tokenizer->NextToken();
        ParseTernaryOperator(negativeBranch);

        if (value.bValue())
        {
            value = positiveBranch;
        }
        else
        {
            value = negativeBranch;
        }
    }
}

void Parser::ParseComparison(Value& value)
{
    ParseAddSubtract(value);

    while (IsBooleanOperation())
    {
        TokenType operationtokenType = tokenizer->tokenType;
        tokenizer->NextToken();

        Value rightPart;
        ParseAddSubtract(rightPart);

        if (operationtokenType == TokenType::Less)
        {
            value.Assign(value.dValue() < rightPart.dValue());
        }

        if (operationtokenType == TokenType::Greater)
        {
            value.Assign(value.dValue() > rightPart.dValue());
        }

        if (operationtokenType == TokenType::LessOrEqual)
        {
            value.Assign(value.dValue() <= rightPart.dValue());
        }

        if (operationtokenType == TokenType::GreaterOrEqual)
        {
            value.Assign(value.dValue() >= rightPart.dValue());
        }

        if (operationtokenType == TokenType::Equal)
        {
            value.Assign(std::abs(value.dValue() - rightPart.dValue()) < PRECISION);
        }

        if (operationtokenType == TokenType::NotEqual)
        {
            value.Assign(std::abs(value.dValue() - rightPart.dValue()) > PRECISION);
        }
    }
}



void Parser::ParseAddSubtract(Value& value)
{
    ParseMultiplyDivide(value);

    // consume all + and - in a loop
    while (tokenizer->tokenType == TokenType::Minus || tokenizer->tokenType == TokenType::Add)
    {
        TokenType operationtokenType = tokenizer->tokenType;
        tokenizer->NextToken();

        // Parse the right hand side of the expression
        Value rightPart;
        ParseMultiplyDivide(rightPart);

        if (value.type == ValueType::BOOL || rightPart.type == ValueType::BOOL)
        {
            throw new std::invalid_argument("Add or substract operations are not defined for Boolean");
        }

        if (operationtokenType == TokenType::Add)
        {
            value.Assign(value.dValue() + rightPart.dValue());
        }
        else if (operationtokenType == TokenType::Minus)
        {
            value.Assign(value.dValue() - rightPart.dValue());
        }
    }
}

void Parser::ParseMultiplyDivide(Value& value)
{
    ParseUnary(value);

    while (tokenizer->tokenType == TokenType::Multiply ||
        tokenizer->tokenType == TokenType::Divide ||
        tokenizer->tokenType == TokenType::Modulus)
    {
        TokenType operationtokenType = tokenizer->tokenType;
        tokenizer->NextToken();

        Value rightPart;
        ParseUnary(rightPart);

        if (value.type == ValueType::BOOL || rightPart.type == ValueType::BOOL)
        {
            throw new std::invalid_argument("Multiplication, division or modulus operations are not defined for Boolean");
        }

        if (operationtokenType == TokenType::Multiply)
        {
            value.Assign(value.dValue() * rightPart.dValue());
        }
        else if (operationtokenType == TokenType::Divide || operationtokenType == TokenType::Modulus)
        {
            if (std::abs(rightPart.dValue()) < PRECISION)
            {
                throw new std::invalid_argument("Division by 0");
            }

            if (operationtokenType == TokenType::Divide)
            {
                value.Assign(value.dValue() / rightPart.dValue());
            }
            else if (operationtokenType == TokenType::Modulus)
            {
                value.Assign(RubyMod(value.dValue(), rightPart.dValue()));
            }
        }
    }
}

void Parser::ParseUnary(Value& value)
{
    // For + just ignore it and go for next unary operation
    if (tokenizer->tokenType == TokenType::Add)
    {
        tokenizer->NextToken();
        ParseUnary(value);
        return;
    }

    if (tokenizer->tokenType == TokenType::Minus)
    {
        tokenizer->NextToken();
        Value rightPart;
        ParseUnary(rightPart);

        value.Assign(-rightPart.dValue());
        return;
    }
    ParsePower(value);
}

void Parser::ParsePower(Value& value)
{
    ParseLeaf(value);
    while (tokenizer->tokenType == TokenType::Power)
    {
        tokenizer->NextToken();
        Value rightPart;
        ParseUnary(rightPart);

        value.Assign(pow(value.dValue(), rightPart.dValue()));
    }
}

void Parser::ParseLeaf(Value& value)
{
    if (tokenizer->tokenType == TokenType::DoubleNumber)
    {
        value.Assign(tokenizer->number);
        tokenizer->NextToken();
        return;
    }

    if (tokenizer->tokenType == TokenType::IntNumber || tokenizer->tokenType == TokenType::HexIntNumber)
    {
        value.Assign(tokenizer->longNumber);
        tokenizer->NextToken();
        return;
    }

    if (tokenizer->tokenType == TokenType::Function)
    {
        std::string fName = tokenizer->stringValue;
        tokenizer->NextToken();

        std::vector<Value> args = ParseFunctionArguments();
        for (Value& v : args)
        {
            if (isinf(v.dValue()))
            {
                throw new std::invalid_argument("Argument is Nan");
            }
        }

        FunctionDescription fNum = Functions::GetFunctionDescription(fName);
        if (args.size() > fNum.maxNumberOfArgs || args.size() < fNum.minNumberOfArgs)
        {
            throw new std::invalid_argument("Invalid number of arguments");
        }

        switch (fNum.id)
        {
            case FunctionNumber::LOG:
                if (args.size() == 1)
                {
                    value.Assign(log(args[0].dValue()));
                }
                else
                {
                    // log(x, base) = log2(x) / log2(base)
                    value.Assign(log2(args[0].dValue()) / log2(args[1].dValue()));
                }
                return;

            case FunctionNumber::LOG10:
                value.Assign(log10(args[0].dValue()));
                return;

            case FunctionNumber::LOG2:
                value.Assign(log2(args[0].dValue()));
                return;
        }
    }

    if (tokenizer->tokenType == TokenType::OpenParens)
    {
        ParseBrackets(value);
        return;
    }

    throw new std::invalid_argument("Unexpected token when parsing leaf"); // + tokenizer.tokenType
}


std::vector<Value> Parser::ParseFunctionArguments()
{
    std::vector<Value> result;

    tokenizer->NextToken();

    Value firstArgument;
    ParseTernaryOperator(firstArgument);
    result.push_back(firstArgument);

    while (tokenizer->tokenType == TokenType::Comma)
    {
        tokenizer->NextToken();
        Value argument;
        ParseTernaryOperator(argument);
        result.push_back(argument);

        tokenizer->NextToken();
    }

    if (result.size() <= 1)
    {
        tokenizer->NextToken();
    }

    return result;
}


void Parser::ParseBrackets(Value& value)
{
    tokenizer->NextToken();

    // Start from the beginning
    ParseTernaryOperator(value);

    if (tokenizer->tokenType != TokenType::CloseParens)
    {
        throw new std::invalid_argument("The closing bracket is missing");
    }
    tokenizer->NextToken();
    return;
}



bool Parser::IsBooleanOperation()
{
    return tokenizer->tokenType == TokenType::Greater ||
        tokenizer->tokenType == TokenType::Less ||
        tokenizer->tokenType == TokenType::GreaterOrEqual ||
        tokenizer->tokenType == TokenType::LessOrEqual ||
        tokenizer->tokenType == TokenType::Equal ||
        tokenizer->tokenType == TokenType::NotEqual;
}


double Parser::RubyMod(double x, double y)
{
    return x - y * floor(x / y);
}



