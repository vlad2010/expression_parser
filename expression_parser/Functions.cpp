#include "Functions.h"

#include <vector>

static std::vector<FunctionDescription> dscs = { {FunctionNumber::LOG, "log", 1, 2}, {FunctionNumber::LOG2, "log2", 1, 1}, {FunctionNumber::LOG10, "log10", 1, 1} };
static const char* nmSpace = "Math";

FunctionDescription Functions::GetFunctionDescription(const std::string& name)
{
    for (int i = 0; i < dscs.size(); i++)
    {
        std::string fullName(nmSpace);
        fullName += ".";
        fullName += dscs[i].name;

        if (dscs[i].name == name.c_str() || fullName == name)
        {
            return dscs[i];
        }
    }

    return FunctionDescription();
}