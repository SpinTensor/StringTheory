#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>

#include "audio_IO.h"
#include "fft.h"
#include "freq_estimator.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FPS 8

const unsigned int mspf = 1000/FPS;

void plot_dataf(int ndata,float* data, int offset, int scale, SDL_Renderer* renderer);
void plot_datad(int ndata,double* data, int offset, int scale, SDL_Renderer* renderer);
void plot_datac(int ndata,fftw_complex* data, int offset, int scale, SDL_Renderer* renderer);

int main(){

   bool tmpbool = (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0);
   if (tmpbool) {
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;

      tmpbool = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 
                                            0, &window, &renderer) == 0;
      if (tmpbool){
         SDL_bool done = SDL_FALSE;
         
         //audio_IO_t audiodata = init_audio(SCREEN_WIDTH);
         audio_IO_t audiodata;
         init_audio(&audiodata, FPS);
         fft_t fftdata = init_fft(audiodata.buffsize, 1.0/(audiodata.audiodevice.freq));

         unsigned int frameStart, frameEnd;
         frameStart = SDL_GetTicks();
         while (!done) {
            SDL_Event event;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            // Audio input stream is automatically updated by callback function
            // copy audio data to fftdata struct
            for (int i=0; i<audiodata.buffsize; i++) {
               fftdata.rawdata[i] = (double) audiodata.buffer[i];
            }
            perform_fft(fftdata);

            plot_dataf(audiodata.buffsize, audiodata.buffer, 1*SCREEN_HEIGHT/4, 100, renderer);
            plot_datad(fftdata.rawsize, fftdata.indata, 2*SCREEN_HEIGHT/4, 100, renderer);
            plot_datac(fftdata.outsize, fftdata.outdata, 3*SCREEN_HEIGHT/4, 10, renderer);

            // search for maximum base frequency index
            int max_freq_i = 0 ;
            double max_amplitude = 0.0 ;
            for (int i = 1; i < fftdata.outsize; i++) {
               double rep = fftdata.outdata[i][0];
               double imp = fftdata.outdata[i][1];
               double tmp_amplitude = rep*rep + imp*imp;
               if (tmp_amplitude > max_amplitude) {
                  max_amplitude = tmp_amplitude;
                  max_freq_i = i;
               }
            }
printf("Frequency: %f %f\n", fftdata.freqs[max_freq_i], estimate_freq(fftdata, 1.0));
fflush(stdout);

            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event)) {
               if (event.type == SDL_QUIT) {
                  done = SDL_TRUE;
               }
            }

            frameEnd=SDL_GetTicks();
            if (frameEnd-frameStart < mspf) {
               SDL_Delay(mspf - (SDL_GetTicks()-frameStart));
            }
            frameStart=SDL_GetTicks();
         }
         free_audio(&audiodata);
      }

      if (renderer) {
         SDL_DestroyRenderer(renderer);
      }
      if (window) {
         SDL_DestroyWindow(window);
      }
   }
   SDL_Quit();

   return 0;
   
}

void plot_dataf(int ndata, float* data, int offset, int scale, SDL_Renderer* renderer){
   SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
   int yold = ((int) (data[0]*scale));
   //int yold = 100;
   for (int i=1; i<ndata; i++){
      int ynew = ((int) (data[i]*scale));
      //int ynew = -yold;
      SDL_RenderDrawLine(renderer, ((i-1)*SCREEN_WIDTH)/(ndata-1), offset-yold, (i*SCREEN_WIDTH)/(ndata-1), offset-ynew);
      yold = ynew;
   }
   return;
}

void plot_datad(int ndata, double* data, int offset, int scale, SDL_Renderer* renderer){
   SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
   int yold = ((int) (data[0]*scale));
   //int yold = 100;
   for (int i=1; i<ndata; i++){
      int ynew = ((int) (data[i]*scale));
      //int ynew = -yold;
      SDL_RenderDrawLine(renderer, ((i-1)*SCREEN_WIDTH)/(ndata-1), offset-yold, (i*SCREEN_WIDTH)/(ndata-1), offset-ynew);
      yold = ynew;
   }
   return;
}

void plot_datac(int ndata, fftw_complex* data, int offset, int scale, SDL_Renderer* renderer){
   SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
   double rep = data[0][0];
   double imp = data[0][1];
   int yold = ((int) (sqrt(rep*rep+imp*imp)*scale));
   for (int i=1; i<ndata; i++){
      rep = data[i][0];
      imp = data[i][1];
      int ynew = ((int) (sqrt(rep*rep+imp*imp)*scale));
      SDL_RenderDrawLine(renderer, ((i-1)*SCREEN_WIDTH)/(ndata-1), offset-yold, (i*SCREEN_WIDTH)/(ndata-1), offset-ynew);
      yold = ynew;
   }
   return;
}
