#include <iostream>
#include "funcbuilder.h"
#include <string>
#include <vector>
#include <stack>
#include <cstring>
#include <cmath>
using namespace std;

FuncBuilder::FuncBuilder(vector<string> expressions, int samples, double min_x, double max_x, bool isPng)
{
    this->expressions = expressions;
    this->samples = samples;
    this->min_x = min_x;
    this->max_x = max_x;
    this->isPng = isPng;
}

double FuncBuilder::calculate(double left, double right, char operation)
{
    switch (operation)
    {
    case '*':
        return left * right;
    case '/':
        if (fabs(right) < 1e-12)
            return INFINITY;
        return left / right;
    case '+':
        return left + right;
    case '-':
        return left - right;
    }
    return 0;
}

int FuncBuilder::priority(char operation)
{
    if (operation == '+' || operation == '-')
        return 1;
    if (operation == '*' || operation == '/')
        return 2;
    else
        return 0;
}

bool FuncBuilder::isOperator(char op)
{
    return op == '-' || op == '/' || op == '+' || op == '*';
}
void FuncBuilder::evaluate(const int width)
{
    if (this->isPng)
    {
        int smp = 2000 / width;
        this->samples = smp * width;
    }
    double step = (this->max_x - this->min_x) / (samples - 1);
    for (string expr : expressions)
    {
        vector<double> results_for_expression;
        for (int k = 0; k < samples; k++)
        {
            double x = this->min_x + k * step;
            this->values = stack<double>();
            this->operators = stack<char>();
            this->sin_functions = stack<string>();

            int expression_size = expr.length();
            bool mapped_to_sin = false;
            for (int i = 0; i < expression_size; i++)
            {
                if (expr[i] == '(')
                {
                    if (i > 0 && expr[i - 1] == 'n')
                        mapped_to_sin = true;
                    else
                        mapped_to_sin = false;
                    this->operators.push(expr[i]);
                }
                else if ('s' == expr[i])
                {
                    this->sin_functions.push("sin");
                    i += 2;
                }
                else if (isdigit(expr[i]) || expr[i] == 'x')
                {
                    double value = 0.0;
                    if (expr[i] == 'x')
                        value = x;
                    else
                    {
                        string str_value = "";
                        while (i < expression_size && (isdigit(expr[i]) || expr[i] == '.'))
                        {
                            str_value += expr[i];
                            i++;
                        }
                        i--;
                        value = stod(str_value);
                    }
                    this->values.push(value);
                }
                else if (expr[i] == ')')
                {
                    while (!this->operators.empty() && this->operators.top() != '(')
                    {
                        double right = this->values.top();
                        values.pop();
                        double left = this->values.top();
                        values.pop();
                        char op = this->operators.top();
                        this->operators.pop();
                        this->values.push(calculate(left, right, op));
                    }
                    if (!this->operators.empty())
                    {
                        this->operators.pop();
                    }
                    if (!this->sin_functions.empty() && mapped_to_sin)
                    {
                        double value = this->values.top();
                        this->values.pop();
                        string func = this->sin_functions.top();
                        this->sin_functions.pop();
                        this->values.push(sin(value));
                    }
                }
                else
                {
                    if (expr[i] == '-' && (i == 0 || expr[i - 1] == '(' || isOperator(expr[i - 1])))
                    {
                        this->values.push(-1);
                        this->operators.push('*');
                    }
                    else
                    {
                        while (!this->operators.empty() && priority(expr[i]) <= priority(this->operators.top()))
                        {
                            double right = this->values.top();
                            values.pop();
                            double left = this->values.top();
                            values.pop();
                            char op = this->operators.top();
                            this->operators.pop();
                            this->values.push(calculate(left, right, op));
                        }
                        this->operators.push(expr[i]);
                    }
                }
            }
            while (!this->operators.empty() || !sin_functions.empty())
            {
                if (!sin_functions.empty())
                {
                    double value = this->values.top();
                    this->values.pop();
                    sin_functions.pop();
                    this->values.push(sin(value));
                }
                if (!this->operators.empty() && this->values.size() >= 2)
                {
                    double right = this->values.top();
                    values.pop();
                    double left = this->values.top();
                    values.pop();
                    char op = this->operators.top();
                    this->operators.pop();
                    this->values.push(calculate(left, right, op));
                }
            }
            results_for_expression.push_back(values.top());
            this->values.pop();
        }
        if (this->results.find(expr) == this->results.end())
            this->results[expr] = results_for_expression;
    }
}

void FuncBuilder::csv_print()
{
    if (expressions.size() == 0)
        return;
    cout << "x,  ";
    for (string expr : expressions)
    {
        cout << expr << ",  ";
    }
    cout << endl;
    double step = (this->max_x - this->min_x) / (samples - 1);
    for (int k = 0; k < samples; k++)
    {
        double x = this->min_x + k * step;
        cout << x << ",  ";
        for (string expr : expressions)
        {
            if (isinf(results[expr].at(k)))
                cout << "Undefined,";
            else
                cout << results[expr].at(k) << ",  ";
        }
        cout << endl;
    }
}

map<string, vector<double>> FuncBuilder::get_results()
{
    return this->results;
}