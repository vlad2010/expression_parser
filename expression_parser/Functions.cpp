#include "Functions.h"

constexpr int FUNC_NUMS = 3;
const char* fnames[FUNC_NUMS] { "log", "log2", "log10" };

FunctionNumber Functions::GetFunctionNumber(const std::string& name)
{
    for (int i = 0; i < FUNC_NUMS; i++)
    {
        if (strcmp(fnames[i], name.c_str()) == 0)
        {
            return (FunctionNumber)i;
        }
    }

    return FunctionNumber::UNDEFINED;
}