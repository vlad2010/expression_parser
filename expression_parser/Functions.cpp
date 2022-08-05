#include "Functions.h"

constexpr int FUNC_NUMS = 3;
const char* fnames[FUNC_NUMS] { "log", "log2", "log10" };
const char* nmSpace = "Math";

FunctionNumber Functions::GetFunctionNumber(const std::string& name)
{
    for (int i = 0; i < FUNC_NUMS; i++)
    {
        std::string fullName(nmSpace);
        fullName += ".";
        fullName += fnames[i];

        if (strcmp(fnames[i], name.c_str()) == 0 || fullName == name)
        {
            return (FunctionNumber)i;
        }
    }

    return FunctionNumber::UNDEFINED;
}