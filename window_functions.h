#ifndef WINDOW_FUNCTIONS_H
#define WINDOW_FUNCTIONS_H

double rectangle_window(int width, double x);
double triangle_window(int width, double x);
double hann_window(int width, double x);
double sine_window(int width, double x);
double nuttall_window(int width, double x);

#endif
