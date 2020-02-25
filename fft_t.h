#ifndef FFT_T_H
#define FFT_T_H

#include <fftw3.h>

typedef struct {
   // size of fouriertransform arrays
   // size of the raw data comming in
   int rawsize;
   // fft_insize is 2*buffsize due to zero-padding
   int insize;
   // input data for the fft
   double* rawdata;
   double* indata;
   // fft_outsize is buffsize because one element ist of complex type
   int outsize;
   // outputdata for the fft
   fftw_complex* outdata;
   double* freqs;
   // fourier transformation plan
   fftw_plan plan;
} fft_t;

#endif
