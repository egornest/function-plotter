#include "parser.h"
#include <vector>
#include <string>
#include "falseinputexception.h"
#include <cstring>
using namespace std;

Parser::Parser(vector<string> input)
{
    this->input = input;
    this->png = false;
    this->samples = 100;
    this->min_x = -10.5;
    this->min_y = -10.5;
    this->max_x = 10.5;
    this->max_y = 10.5;
    this->height = 500;
    this->width = 500;
    this->image_name = "image.png";
}

const char *Parser::parse_whitespase(const char *argument)
{
    if (argument == nullptr)
        return nullptr;
    while (*argument == ' ' || *argument == '\n' || *argument == '\"')
        ++argument;
    return argument;
}

bool Parser::is_Valid_Input()
{
    bool isValid = true;
    try
    {
        choose_parse_by_input();
    }
    catch (const FalseInputException &e)
    {
        cerr << e.what() << '\n';
        isValid = false;
    }
    return isValid;
}

void Parser::choose_parse_by_input()
{
    auto it = input.begin();
    int count = 0;
    while (it != input.end() && *it != "")
    {
        if (*it == "--expr")
        {
            validate_expression(input.erase(it));
            count++;
        }
        else if (*it == "--samples")
            this->samples = convert_to_int("samples", input.erase(it));
        else if (*it == "--img-width")
            this->width = convert_to_int("width", input.erase(it));
        else if (*it == "--img-height")
            this->height = convert_to_int("height", input.erase(it));
        else if (*it == "--min-x")
            this->min_x = convert_to_double("min-x", input.erase(it));
        else if (*it == "--max-x")
            this->max_x = convert_to_double("max-x", input.erase(it));
        else if (*it == "--max-y")
            this->max_y = convert_to_double("max-y", input.erase(it));
        else if (*it == "--min-y")
            this->min_y = convert_to_double("min-y", input.erase(it));
        else if (*it == "--png")
        {
            this->png = true;
            parse_png_name(input.erase(it));
        }
        else if (*it == "--help" || *it == "-h")
        {
            print_help(it);
            count++;
            break;
        }
        else
            throw FalseInputException("Unknown input " + *it);
    }
    if (count == 0)
        throw FalseInputException("You must enter expression!");
}

void Parser::parse_png_name(vector<string>::iterator it)
{
    string temp_name = *it;
    string name = "";
    bool isPng_in_name = false;
    int size = temp_name.length();
    for (int i = 0; i < size; i++)
    {
        bool restricted = temp_name[i] == '/' || temp_name[i] == '*' || temp_name[i] == '?' 
        || temp_name[i] == '[' || temp_name[i] == ']' || temp_name[i] == '|' || temp_name[i] == '`' 
        || temp_name[i] == '~' || temp_name[i]=='\\' || temp_name[i]=='\'' || temp_name[i]==':';
        if ((isPng_in_name && i < size) || restricted)
            throw FalseInputException("you need to enter dataname and dataname must end with .png\ndataname can`t have restricted symbols such as \'/\'");
        name += temp_name[i];
        if (i < size && temp_name[i] == '.')
        {
            i++;
            string png = temp_name.substr(i, i + 3);
            if (strncmp(png.c_str(), "png", 3) == 0)
            {
                isPng_in_name = true;
                name += "png";
            }
            i += 3;
        }
    }
    if (!isPng_in_name)
        throw FalseInputException("you need to enter dataname and dataname must end with .png");
    this->image_name = name;
    this->input.erase(it);
}

const char *Parser::parse_arithmetic(const char *argument, string &expr)
{
    switch (*argument)
    {
    case '*':
        expr += *argument;
        break;
    case '/':
        expr += *argument;
        break;
    case '+':
        expr += *argument;
        break;
    case '-':
        expr += *argument;
        break;
    default:
        return nullptr;
    }
    return ++argument;
}

bool Parser::isDigit(char c) const
{
    return ('0' <= c && c <= '9') || c == '.';
}

const char *Parser::parse_decimal_value(const char *argument, string &expr)
{
    if (!isDigit(*argument))
        return nullptr;
    while (isDigit(*argument))
    {
        expr += *argument;
        ++argument;
    }
    return argument;
}

const char *Parser::parse_brackets(const char *argument, string &expr)
{
    while (argument != nullptr && *argument != ')' && *argument != 0)
    {
        argument = parse_whitespase(argument);
        argument = parse_expression(argument, expr);
        argument = parse_whitespase(argument);
    }
    if (argument == nullptr || *argument == 0)
        return nullptr;
    expr += *argument;
    argument = parse_whitespase(++argument);
    return argument;
}

const char *Parser::parse_expression(const char *argument, string &expr)
{
    while (argument != nullptr && *argument != 0 && *argument != ')')
    {
        argument = parse_whitespase(argument);
        argument = choose_parse_by_expression(argument, expr);
        if (argument == nullptr)
            break;
        argument = parse_whitespase(argument);
        if (*argument != ')' && *argument != 0 && *(--argument) != '-')
        {
            argument = parse_arithmetic(++argument, expr);
            if (argument == nullptr || *argument == 0 || *argument == ')')
                return nullptr;
        }
        else if (*argument == '-') 
            ++argument;
    }
    if (argument == nullptr || *(--argument) == '(')
        return nullptr;
    return ++argument;
}

const char *Parser::parse_function(const char *argument, string &expr)
{
    if (strncmp(argument, "sin", 3) == 0)
    {
        argument += 3;
    }
    else
        return nullptr;
    argument = parse_whitespase(argument);
    string inner_expr = "";
    if (*argument != '(')
        return nullptr;
    expr += "sin(";
    argument = parse_expression(++argument, inner_expr);
    if (argument == nullptr || *argument != ')')
        return nullptr;
    expr += inner_expr + ')';
    ++argument;
    return argument;
}

const char *Parser::choose_parse_by_expression(const char *argument, string &expr)
{
    if (*argument >= '0' && *argument <= '9')
    {
        return parse_decimal_value(argument, expr);
    }
    if (strncmp(argument, "sin", 3) == 0)
        return parse_function(argument, expr);
    switch (*argument)
    {
    case '(':
        expr += *argument;
        return parse_brackets(++argument, expr);
    case 'x':
        expr += *argument;
        return ++argument;
    case '-':
        expr += *argument;
        return ++argument;
    default:
        break;
    }
    return nullptr;
}

void Parser::validate_expression(vector<string>::iterator pos)
{
    if ((*pos).size() == 0)
        throw FalseInputException("Expression can`t be empty ");
    const char *argument = (*pos).c_str();
    string expr = "";
    argument = parse_expression(argument, expr);

    if (argument != nullptr && *argument == 0)
    {
        expressions.push_back(expr);
        input.erase(pos);
        return;
    }
    throw FalseInputException("false input for --expr: " + *pos + " in position " + to_string(expr.length()));
}

int Parser::convert_to_int(string name, vector<string>::iterator pos)
{
    try
    {
        int value = stoi(*pos);
        input.erase(pos);
        return value;
    }
    catch (const invalid_argument &e)
    {
        throw FalseInputException("false type in " + name + "with " + *pos + " expected [Number]");
    }
}

double Parser::convert_to_double(string name, vector<string>::iterator pos)
{
    try
    {
        double value = stod(*pos);
        input.erase(pos);
        return value;
    }
    catch (const invalid_argument &e)
    {
        throw FalseInputException("false type in " + name + " with " + *pos + "\n expected [Number]");
    }
}

void Parser::print_help(vector<string>::iterator it)
{
    cout << R"(usage : func_plotter [ options ]
Plots function expressions .
-h --help,Print this message
--png file_name.png      Render plot into given png file
--samples [ number ]     Number of samples in CSV output\n [ default : 100]
--min-x [ number ]       Min . x value to plot [ default : -10.5]
--max-x [ number ]       Max . x value to plot [ default : 10.5]
--min-y [ number ]       Min . y value to plot [ default : -10.5]
--max-y [ number ]       Max . y value to plot [ default : 10.5]
--img-width [ number ]   Width of the png [ default : 500]
--img-height [ number ]  Height of the png [ default : 500]
--expr [ str ]          Plot the given expression. Can be specified multiple times. Must be specified at least once.)"
         << endl;
    input.erase(it);
}

vector<string> Parser::get_expressions()
{
    return this->expressions;
}
int Parser::get_samples()
{
    return this->samples;
}
double Parser::get_max_x()
{
    return this->max_x;
}
double Parser::get_max_y()
{
    return this->max_y;
}
double Parser::get_min_x()
{
    return this->min_x;
}
double Parser::get_min_y()
{
    return this->min_y;
}
bool Parser::isPng()
{
    return this->png;
}
int Parser::get_height()
{
    return this->height;
}
int Parser::get_width()
{
    return this->width;
}
string Parser::get_image_name()
{
    return this->image_name;
}