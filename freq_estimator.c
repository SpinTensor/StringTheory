#include "fft_t.h"

double estimate_freq(fft_t fftdata) {
   // search for maximum base frequency index
   int max_ampl_i = 0 ;
   double max_amplitude = 0.0 ;
   for (int i = 1; i < fftdata.outsize; i++) {
      double rep = fftdata.outdata[i][0];
      double imp = fftdata.outdata[i][1];
      double tmp_amplitude = rep*rep + imp*imp;
      if (tmp_amplitude > max_amplitude) {
         max_amplitude = tmp_amplitude;
         max_ampl_i = i;
      }
   }
   if (max_ampl_i == 0) {return 0.0;}
   // compute the center of amplitude like a "center of mass"
   // the amplitude square is the mass
   double y = max_amplitude;
   double sumy = y;
   double sumxy = fftdata.freqs[max_ampl_i]*y;

   // search the left most index whichs amplitude is still above 10% of the maximum amplitude
   int low_bound_i = max_ampl_i;
   y = max_amplitude;
   while ((y >= 0.1*max_amplitude) && low_bound_i > 0) {
      low_bound_i--;
      double rep = fftdata.outdata[low_bound_i][0];
      double imp = fftdata.outdata[low_bound_i][1];
      y = rep*rep + imp*imp;
      sumy += y;
      sumxy += fftdata.freqs[low_bound_i]*y;
   }

   // search the right most index whichs amplitude is still above 10% of the maximum amplitude
   int high_bound_i = max_ampl_i;
   y = max_amplitude;
   while ((y >= 0.1*max_amplitude) && low_bound_i < fftdata.outsize-1) {
      high_bound_i++;
      double rep = fftdata.outdata[high_bound_i][0];
      double imp = fftdata.outdata[high_bound_i][1];
      y = rep*rep + imp*imp;
      sumy += y;
      sumxy += fftdata.freqs[high_bound_i]*y;
   }

   sumy = (sumy > 0.0) ? sumy : 1.0;
   return sumxy/sumy;
}
