#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include "draw.h"
#include "funcbuilder.h"
#include <map>
using namespace std;

template <class F>
struct AtExit
{
    explicit AtExit(F f) : f(f) {}
    AtExit(const AtExit &) = delete;
    AtExit &operator=(const AtExit &) = delete;
    AtExit(AtExit &&) = delete;
    AtExit &operator=(AtExit &&) = delete;
    ~AtExit() { f(); }
    F f;
};

Draw::Draw(FuncBuilder builder, int width, int height, double min_x, double max_x, double min_y, double max_y, string image_name)
    : width(width), height(height),
      min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y),
      builder(builder), image_name(image_name)
{
}


int Draw::get_x_axis_pixel() const
{
    if (min_y <= 0 && max_y >= 0)
        return height - ((0 - min_y) / (max_y - min_y) * height);
    else if (min_y > 0)
        return height; 
    else
        return 0; 
}

int Draw::get_y_axis_pixel() const
{
    if (min_x <= 0 && max_x >= 0)
        return (0 - min_x) / (max_x - min_x) * width;
    else if (min_x > 0)
        return 0;
    else
        return width;
}

void Draw::draw_all()
{
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
    AtExit surface_cleanup([&]
                           { cairo_surface_destroy(surface); });
    {
        cairo_t *cr = cairo_create(surface);
        AtExit cr_cleanup([&]
                          { cairo_destroy(cr); });
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_paint(cr);
        draw_axis(cr);
        draw_functions(cr);
    }
    cairo_surface_write_to_png(surface, image_name.c_str());
}

void Draw::draw_axis(cairo_t *cr)
{
    int x_axis_y = get_x_axis_pixel();
    int y_axis_x = get_y_axis_pixel();

    cairo_move_to(cr, 0, x_axis_y);
    cairo_line_to(cr, width, x_axis_y);
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke(cr);

    cairo_move_to(cr, y_axis_x, height);
    cairo_line_to(cr, y_axis_x, 0);
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_stroke(cr);

    cairo_set_font_size(cr, 12);
    int number_of_coordinates = width >= 500 ? 20 : 10;
    int x_start = ceil(min_x) == min_x ? ceil(min_x + 0.5) : ceil(min_x);
    int x_end = floor(max_x) == max_x ? floor(max_x + 0.5) : floor(max_x);
    int x_step = max(1, (int)ceil((double)(x_end - x_start) / number_of_coordinates));
    int y_start = ceil(min_y) == min_y ? ceil(min_y + 0.5) : ceil(min_y);
    int y_end = floor(max_y) == max_y ? floor(max_y + 0.5) : floor(max_y);
    int y_step = max(1, (int)ceil((double)(y_end - y_start) / number_of_coordinates));

    for (int x = x_start; x <= x_end; x += x_step)
    {
        double pos = (x - min_x) / (max_x - min_x) * width;
        if (abs(x) < 1e-6)
            continue;
        string text = to_string(x);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_move_to(cr, pos - extents.width / 2, x_axis_y + 15);
        cairo_show_text(cr, text.c_str());
        cairo_move_to(cr, pos, x_axis_y - 5);
        cairo_line_to(cr, pos, x_axis_y + 5);
        cairo_stroke(cr);
    }
    const char *x_y = "x";
    cairo_move_to(cr, width - 15, x_axis_y - 5);
    cairo_show_text(cr, x_y);
    for (int y = y_start; y <= y_end; y += y_step)
    {
        double pos = height - (y - min_y) / (max_y - min_y) * height;
        string text = to_string(y);
        if (abs(y) < 1e-6)
            text = "0";
        cairo_text_extents_t extents;
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_move_to(cr, pos - extents.height / 2, x_axis_y + 15);
        cairo_move_to(cr, y_axis_x + 5, pos);
        cairo_show_text(cr, text.c_str());
        cairo_move_to(cr, y_axis_x - 5, pos);
        cairo_line_to(cr, y_axis_x + 5, pos);
        cairo_stroke(cr);
    }
    x_y = "y";
    cairo_move_to(cr, y_axis_x + 5, 15);
    cairo_show_text(cr, x_y);
}

void Draw::draw_functions(cairo_t *cr)
{
    srand(time(nullptr));
    int index = 0;
    map<string, vector<double>> map_of_results = this->builder.get_results();
    for (auto it = map_of_results.begin(); it != map_of_results.end(); it++)
    {
        double r = 0.4 + (rand() / (double)RAND_MAX) * 0.5;
        double g = 0.4 + (rand() / (double)RAND_MAX) * 0.5;
        double b = 0.4 + (rand() / (double)RAND_MAX) * 0.5;
        cairo_set_source_rgb(cr, r, g, b);
        cairo_set_line_width(cr, 2);
        cairo_new_path(cr);
        vector<double> results = it->second;
        double step = (this->max_x - this->min_x) / (results.size() - 1);
        int size = results.size();
        bool move_needed = true;
        for (int i = 0; i < size; i++)
        {
            double x = this->min_x + i * step;
            double y = results[i];
            bool move_from_above_below = (i > 0 && y > this->max_y && (results[i - 1] < this->min_y || results[i-1]<0)) 
            || (i > 0 && y < this->min_y && (results[i - 1] > this->max_y || results[i-1]>0))
            || (i > 0 && y < 0 && results[i - 1] > this->max_y)
            || (i > 0 && y > 0 && results[i - 1] < this->min_y);
            if (isinf(y) || move_from_above_below)
                move_needed = true;
            double pixel_x = (x - this->min_x) / (this->max_x - this->min_x) * this->width;
            double pixel_y = this->height - (y - this->min_y) / (this->max_y - this->min_y) * this->height;

            if (move_needed && !isinf(y))
            {
                cairo_move_to(cr, pixel_x, pixel_y);
                move_needed = false;
            }
            else
            {
                cairo_line_to(cr, pixel_x, pixel_y);
            }
        }
        cairo_stroke(cr);
        cairo_set_font_size(cr, 20);
        cairo_move_to(cr, 25, 25 + index * 20);
        cairo_show_text(cr, it->first.c_str());
        index++;
    }
}