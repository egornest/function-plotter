#ifndef DRAW_H
#define DRAW_H
#include <cairo/cairo.h>
#include "funcbuilder.h"
#include <string>
class Draw
{
private:
    int width;
    int height;
    double min_x, min_y, max_x, max_y;
    FuncBuilder builder;
    std::string image_name;
    void draw_axis(cairo_t *cr);
    void draw_functions(cairo_t *cr);
    int get_x_axis_pixel()const;
    int get_y_axis_pixel()const;
public:
    Draw(FuncBuilder builder, int width, int height, double min_x, double max_x, double min_y, double max_y, std::string image_name);
    void draw_all();
};


#endif