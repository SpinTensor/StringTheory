#ifndef WINDOW_FUNCTIONS_H
#define WINDOW_FUNCTIONS_H

typedef enum {
   rectangle,
   triangle,
   hann,
   sine,
   nuttall
} winfunc_t;

double rectangle_window(int width, double x);
double triangle_window(int width, double x);
double hann_window(int width, double x);
double sine_window(int width, double x);
double nuttall_window(int width, double x);

void apply_window(winfunc_t winfunc, int winsize, double *indata, double *outdata);

#endif
