#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, (int) round(x), (int) round(y), c);
}

image nn_resize(image im, int w, int h)
{
    int i, j, k;
    float a_x, a_y, b_x, b_y, pixel;
    image new_img = make_image(w, h, im.c);
    a_x = (float)im.w / w;
    b_x = -0.5 + 0.5 * a_x;
    a_y = (float)im.h / h;
    b_y = -0.5 + 0.5 * a_y;
    for ( i = 0; i < w; i++ ) {
        for ( j = 0; j < h; j++ ) {
            for ( k = 0; k < im.c; k++ ) {
                pixel = nn_interpolate(im, a_x * i + b_x, a_y * j + b_y, k);
                set_pixel(new_img, i, j, k, pixel);
            }
        }
    }
    return new_img;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    /*         top_middle
        *--------------------* pixel
        |  left    | right   |
        |          |         | top
        |          |         |
        |----------*---------- 
        |          |         |
        |          |         | bottom
        *--------------------*
                bottom_middle
    */
    float left, right, top, bottom, top_middle, bottom_middle, top_right_pixel,
          top_left_pixel, bottom_right_pixel, bottom_left_pixel, pixel;
    int floor_x, floor_y, ceil_x, ceil_y;
    floor_x = (int) floorf(x);
    ceil_x = (int) ceilf(x);
    floor_y = (int) floorf(y);
    ceil_y = (int) ceilf(y);
    left = x - floor_x;
    right = ceil_x - x;
    top = y - floor_y;
    bottom = ceil_y - y;
    // Get the weighted average of the top right pixel and the top left pixel
    // Let the weight be the swapped distance of the point from the pixels.
    top_left_pixel = get_pixel(im, floor_x, floor_y, c);
    top_right_pixel = get_pixel(im, ceil_x, floor_y, c);
    top_middle = left * top_right_pixel + right * top_left_pixel;

    // Do the same but for the bottom pixels.
    bottom_left_pixel = get_pixel(im, floor_x, ceil_y, c);
    bottom_right_pixel = get_pixel(im, ceil_x, ceil_y, c);
    bottom_middle = left * bottom_right_pixel + right * bottom_left_pixel;

    // Do the same with the linearly interpolated pixels.
    pixel = bottom * top_middle + top * bottom_middle;
    return pixel;
}

image bilinear_resize(image im, int w, int h)
{
    int i, j, k;
    float a_x, a_y, b_x, b_y, pixel;
    image new_img = make_image(w, h, im.c);
    a_x = (float)im.w / w;
    b_x = -0.5 + 0.5 * a_x;
    a_y = (float)im.h / h;
    b_y = -0.5 + 0.5 * a_y;
    for ( i = 0; i < w; i++ ) {
        for ( j = 0; j < h; j++ ) {
            for ( k = 0; k < im.c; k++ ) {
                pixel = bilinear_interpolate(im, a_x * i + b_x, a_y * j + b_y, k);
                set_pixel(new_img, i, j, k, pixel);
            }
        }
    }
    return new_img;
}

