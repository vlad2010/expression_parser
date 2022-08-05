#include <stdio.h>

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Value.h"
#include "Parser.h"

using namespace std;

// Test case structure
struct TestCase
{
    string expression;
    string type;
    string expectedValue;

    Value actualResult;
    bool exceptionWasThrown = false;
    char templateChar = 0;
    long templateValue;
    bool exceptionExpected;

    bool isValid = false;
};

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");

bool checkResults(TestCase& test);
TestCase getTestCaseFromLine(string line);
bool executeTestCase(TestCase& test);

const std::string exceptionType = "exception";
const std::string doubleType = "double";
const std::string boolType = "bool";

std::vector<string> getComponents(string& s);

int main()
{
    const char* tests = "./test_cases.txt";

    std::ifstream infile(tests);
    std::string line;
    int total_lines = 0;
    int passedTests = 0;
    int failedTests = 0;
    while (std::getline(infile, line))
    {
        trim(line);
        if (line.size() > 0 && line.at(0) != '#')
        {
            TestCase test = getTestCaseFromLine(line);

            if (!test.isValid)
            {
                cout << "ERROR: Not enough components in test case line: " << line << endl;
                failedTests++;
                continue;
            }

            executeTestCase(test);

            if (checkResults(test))
            {
                passedTests++;
            }
            else
            {
                cout << "Error in Line: " << line << endl;
                failedTests++;
            }

            total_lines++;
        }
    }

    cout << "Total test cases: " << total_lines << endl;
    cout << "Tests passed: " << passedTests << endl;
    cout << "Tests failed: " << failedTests << endl;

}

bool executeTestCase(TestCase& test)
{
    try
    {
        if (test.templateChar!=0)
        {
            Parser ps(test.expression, test.templateChar, test.templateValue);
            test.actualResult = ps.ParseExpression();
        }
        else
        {
            Parser ps(test.expression);
            test.actualResult = ps.ParseExpression();
        }
    }
    catch (...)
    {
        test.exceptionWasThrown = true;
    }

    return true;
}

TestCase getTestCaseFromLine(string line)
{
    TestCase test;

    std::vector<string> components = getComponents(line);
    if (components.size() < 2)
    {
        return test;
    }

    test.exceptionExpected = false;

    test.expression = components[0];
    test.type = trim(components[1]);

    if (exceptionType.compare(test.type) == 0)
        test.exceptionExpected = true;

    if (components.size() >= 3)
    {
        test.expectedValue = trim(components[2]);
    }

    if (components.size() >= 5)
    {
        test.templateChar = trim(components[3]).at(0);
        test.templateValue = stol(trim(components[4]));
    }

    test.isValid = true;
    return test;
}

bool checkResults(TestCase &test)
{
    if (test.exceptionWasThrown)
    {
        if (test.exceptionExpected)
            return true;
        else
            return false;
    }
    else
    {
        if (doubleType.compare(test.type) == 0)
        {
            double expRes = std::stod(test.expectedValue);
            if (abs(expRes - test.actualResult.dValue()) > PRECISION)
            {
                cout << "ERROR: Wrong double res " << test.actualResult.dValue() << "  Expected res: " << expRes << endl;
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            if (boolType.compare(test.type) == 0)
            {
                bool expRes = (test.expectedValue.compare("true") == 0) ? true : false;
                if (test.actualResult.bValue() != expRes)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
    }
    return false;
}


std::string& ltrim(std::string& str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}

std::vector<string> getComponents(string& str)
{
    std::vector<string> res;
    stringstream s_stream(str);
    while (s_stream.good())
    {
        string substr;
        getline(s_stream, substr, ',');
        res.push_back(substr);
    }

    return res;
}