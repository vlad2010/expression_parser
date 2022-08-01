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

class Functions
{
public:
    static FunctionNumber GetFunctionNumber(const std::string& name);
};

