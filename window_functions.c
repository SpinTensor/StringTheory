#include <math.h>

#include "mp_constants.h"

double rectangle_window(int width, double x) {
   return 1.0+0.0*width*x;
}

double triangle_window(int width, double x) {
   double y = (x - 0.5*width);
   y /= 0.5*(width+1);
   y = (y < 0.0) ? -y : y ;
   return 1.0 - y;
}

double hann_window(int width, double x) {
   double scale= twopi/((double)width);
   return 0.5 - 0.5*(cos(x*scale));
}

double sine_window(int width, double x) {
   double scale= pi/((double)width);
   return sin(x*scale);
}

double nuttall_window(int width, double x) {
   const double a0 = 0.355768;
   const double a1 = 0.487396;
   const double a2 = 0.144232;
   const double a3 = 0.012604;

   double s = twopi*x/width;
   return a0 - a1*cos(s) + a2*cos(2.0*s) - a3*cos(3*s);
}
