#pragma once

#include <string>
#include "Value.h"

enum class FunctionNumber
{
    UNDEFINED = -1,
    LOG = 0,
    LOG2,
    LOG10,
};

struct FunctionDescription
{
    FunctionNumber id = FunctionNumber::UNDEFINED;
    std::string name;
    unsigned int minNumberOfArgs = 0;
    unsigned int maxNumberOfArgs = 0;
};

class Functions
{
public:
    static FunctionDescription GetFunctionDescription(const std::string& name);
};

