#ifndef  __PARSER__H__
#define __PARSER__H__

#include <string>
#include <memory>
#include "Tokenizer.h"
#include "Value.h"

const double PRECISION = 0.000000001;

class Parser
{
public:
    Parser(std::string& expr);
    Parser(std::string& expr, char templateSymbol, long templateValue);
    Value ParseExpression();

private:
    std::unique_ptr<Tokenizer> tokenizer;

    void ParseTernaryOperator(Value& value);
    void ParseComparison(Value& value);
    void ParseAddSubtract(Value& value);
    void ParseMultiplyDivide(Value& value);
    void ParseUnary(Value& value);
    void ParsePower(Value& value);
    void ParseLeaf(Value& value);

    bool IsBooleanOperation();
    double RubyMod(double x, double y);
};


#endif 


