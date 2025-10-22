#ifndef FUNCBUILDER_H
#define FUNCBUILDER_H

#include <string>
#include <vector>
#include <map>
#include <stack>

class FuncBuilder
{
private:
    int samples;
    double min_x;
    double max_x;
    bool isPng;
    std::map<std::string, std::vector<double>> results;
    std::vector<std::string> expressions;
    std::stack<double> values;
    std::stack<char> operators;
    std::stack<std::string> sin_functions;
    double calculate(double left, double right, char operation);
    int priority(char operation);
    bool isOperator(char op);
public:
    FuncBuilder(std::vector<std::string> expressions, int samples, double min_x, double max_x, bool isPng);
    void evaluate(const int height);
    void csv_print();
    std::map<std::string, std::vector<double>> get_results();
};
#endif
