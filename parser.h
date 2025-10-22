#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
using namespace std;
class Parser
{
private:
    vector<string> input;
    vector<string> expressions;
    bool png;
    double min_x, min_y, max_x, max_y;
    int width, height;
    int samples;
    string image_name;
    bool isDigit(char c) const;
    double convert_to_double(string name, vector<string>::iterator pos);
    int convert_to_int(string name,vector<string>::iterator pos);
    void print_help(vector<string>::iterator it);
    void choose_parse_by_input();
    const char *parse_decimal_value(const char *argument, string &expr);
    const char *parse_arithmetic(const char *argument, string &expr);
    const char *parse_whitespase(const char *argument);
    const char *choose_parse_by_expression(const char *argument, string &expr);
    const char *parse_brackets(const char *argument, string &expr);
    const char *parse_sin(const char *argument, string &expr);
    const char *parse_expression(const char *argument, string &expr);
    const char *parse_function(const char *argument, string &expr);
    void parse_png_name(vector<string>::iterator it);
public:
    Parser(vector<string> input);
    void validate_expression(vector<string>::iterator pos);
    bool is_Valid_Input();
    vector<string> get_expressions();
    int get_samples();
    int get_width();
    int get_height();
    double get_max_x();
    double get_max_y();
    double get_min_x();
    double get_min_y();
    bool isPng();
    string get_image_name();
};

#endif