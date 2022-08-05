#ifndef  __VALUE__H__
#define __VALUE__H__

union ValueObject
{
    bool   boolValue;
    double doubleValue;
    long   longValue;
};

enum ValueType
{
    BOOL,
    DOUBLE,
    LONG  // only double types actually used
};

class Value
{
public:
    Value();
    Value(double init);

    ValueObject value;
    ValueType type;

    void Assign(bool b);
    void Assign(double b);
    void Assign(long b);

    bool bValue();
    double dValue();
    long lValue();

    void print();
};

#endif

