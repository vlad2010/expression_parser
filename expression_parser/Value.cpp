#include "Value.h"

#include <stdio.h>

Value::Value()
{
    Assign(0.0);
}

Value::Value(double init)
{
    Assign(init);
}

void Value::Assign(bool val)
{
    value.boolValue = val;
    type = ValueType::BOOL;
}

void Value::Assign(double val)
{
    value.doubleValue = val;
    type = ValueType::DOUBLE;
}

void Value::Assign(long val)
{
    value.longValue = val;
    type = ValueType::LONG;
}

bool Value::bValue()
{
    return value.boolValue;
}

double Value::dValue()
{
    return value.doubleValue;
}

long Value::lValue()
{
    return value.longValue;
}

void Value::print()
{
    switch (type)
    {
    case ValueType::BOOL:
        printf("BOOL value. %s \n", (value.boolValue ? "true" : "else"));
        break;
    case ValueType::LONG:
        printf("LONG value. %ld \n", value.longValue);
        break;
    case ValueType::DOUBLE:
        printf("DOUBLE value. %f \n", value.doubleValue);
        break;
    default:
        printf("Error: Unknown value type\n");
    }

}