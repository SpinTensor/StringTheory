#ifndef HANDLEAUDIO_H
#define HANDLEAUDIO_H

#include <SDL2/SDL.h>
#include "fftw3.h"

typedef struct {
   // buffer size for audio input
   int buffsize;
   // buffer for audio input
   float* audiobuffer;
   //
   // SDL audio device information
   SDL_AudioSpec wanteddev;
   SDL_AudioSpec audiodevice;
   SDL_AudioDeviceID devid_in;
   //
   // size of fouriertransform arrays
   // fft_insize is 2*buffsize due to zero-padding
   int fft_insize;
   // input data for the fft
   double* fft_indata;
   // fft_outsize is buffsize because one element ist of complex type
   int fft_outsize;
   // outputdata for the fft
   fftw_complex* fft_outdata;
   // fourier transformation plan
   fftw_plan fftw_plan;
} audio_t;


audio_t init_audio(int buffersize);

void get_audio_data(audio_t audiodata);

void free_audio(audio_t* audiodata);



#endif
