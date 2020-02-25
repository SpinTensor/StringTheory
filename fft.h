#ifndef FFT_H
#define FFT_H

#include "fft_t.h"

fft_t init_fft(int fft_size, double dt);

void perform_fft(fft_t fftdata);

void free_fft(fft_t* fftdata);

#endif
