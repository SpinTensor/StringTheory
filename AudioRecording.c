#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>

#include "fftw3.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FPS 50

const unsigned int mspf = 1000/FPS;

static SDL_AudioDeviceID devid_in = 0;
static SDL_AudioSpec want;
static SDL_AudioSpec spec;

void plot_data(Uint8* data, int offset, int scale, SDL_Renderer* renderer);

int main(){

   double pi = acos(0.0);

   bool tmpbool;

   Uint8* buffer=(Uint8*)malloc(SCREEN_WIDTH*sizeof(Uint8));
   Uint8* fftbuffer=(Uint8*)malloc(SCREEN_WIDTH*sizeof(Uint8));
   for (int i=0; i<SCREEN_WIDTH; i++) {
      buffer[i] = 0;
   }

   int fftsize = 2*SCREEN_WIDTH;
   double* indata = fftw_alloc_real(fftsize);
   fftw_complex* outdata = fftw_alloc_complex(fftsize/2);
   for (int i=0; i<fftsize; i++) {
      indata[i] = 0.0;
   }
   for (int i=0; i<fftsize/2; i++) {
      outdata[0][i] = 0.0;
      outdata[1][i] = 0.0;
   }

   fftw_plan audiospec_plan = 
      fftw_plan_dft_r2c_1d(fftsize,         //number of elements
                           indata,          //input data
                           outdata,         //output data
                           FFTW_ESTIMATE);   //FFTW algorithm

   tmpbool = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0;
   if (tmpbool) {
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;

      tmpbool = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 
                                            0, &window, &renderer) == 0;
      if (tmpbool){
         SDL_bool done = SDL_FALSE;
         
         // open input audio device
         SDL_zero(want);
         want.freq = 41000;
         want.format = AUDIO_S8;
         want.channels = 1;
         want.samples = SCREEN_WIDTH;
         want.callback = NULL;
         SDL_zero(spec);

         devid_in = SDL_OpenAudioDevice(NULL, SDL_TRUE, &want, &spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
         printf("freq: %d\n", spec.freq);
         printf("format: %d\n", spec.format);
         printf("channels: %d\n", spec.channels);
         printf("silence: %d\n", spec.silence);
         printf("samples: %d\n", spec.samples);
         SDL_PauseAudioDevice(devid_in, SDL_FALSE);
         //SDL_ClearQueuedAudio(devid_in);


         unsigned int frameStart, frameEnd;
         frameStart = SDL_GetTicks();
         while (!done) {
            SDL_Event event;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            //SDL_PauseAudioDevice(devid_in, SDL_FALSE);
            SDL_DequeueAudio(devid_in, buffer, SCREEN_WIDTH*sizeof(Uint8));
            //SDL_PauseAudioDevice(devid_in, SDL_TRUE);
            SDL_ClearQueuedAudio(devid_in);

            //printf("%d\n", buffer[0]);
            plot_data(buffer, 7*SCREEN_HEIGHT/8, 1, renderer);
            double nr = 1.0/SCREEN_WIDTH;
            for (int i=0;i<SCREEN_WIDTH; i++) {
               double w = 0.5 - 0.5*cos(2.0*pi*i*nr);
               indata[i] = buffer[i]-spec.silence;
               indata[i] *= w;
            }
            fftw_execute(audiospec_plan);
            for (int i=0;i<SCREEN_WIDTH; i++) {
               double tmp;
               tmp = outdata[i][0]*outdata[i][0]+outdata[i][1]*outdata[i][1];
               fftbuffer[i] = 0.1*sqrt(tmp);
            }
            plot_data(fftbuffer, 3*SCREEN_HEIGHT/8, 1, renderer);

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
      }

      if (renderer) {
         SDL_DestroyRenderer(renderer);
      }
      if (window) {
         SDL_DestroyWindow(window);
      }
      SDL_PauseAudioDevice(devid_in, SDL_TRUE);
      SDL_CloseAudioDevice(devid_in);
   }
   SDL_Quit();

   return 0;
   
}

void plot_data(Uint8* data, int offset, int scale, SDL_Renderer* renderer){
   SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
   int yold = offset - ((int) (data[0]/scale));
   for (int i=1; i<SCREEN_WIDTH; i++){
      int ynew = offset - ((int) (data[i]/scale));
      SDL_RenderDrawLine(renderer, i-1, yold, i, ynew);
      yold = ynew;
   }
   return;
}
