#include <iostream>
#include "parser.h"
#include "funcbuilder.h"
#include "draw.h"
#include <vector>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    vector<string> input;
    if (argc == 1)
    {
        input.push_back("--help");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            string arg = argv[i];
            input.push_back(arg);
        }
    }
    Parser parser(input);
    bool isValid = parser.is_Valid_Input();
    FuncBuilder func_builder(parser.get_expressions(), parser.get_samples(), parser.get_min_x(), parser.get_max_x(), parser.isPng());
    func_builder.evaluate(parser.get_width());
    if (isValid)
    {
        if (parser.isPng())
        {
            Draw draw(func_builder, parser.get_width(), parser.get_height(),
                      parser.get_min_x(), parser.get_max_x(), parser.get_min_y(), parser.get_max_y(), parser.get_image_name());
            draw.draw_all();
        }
        else
            func_builder.csv_print();
    }
}