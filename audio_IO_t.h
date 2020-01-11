#ifndef AUDIO_IO_T_H
#define AUDIO_IO_T_H

#include <SDL2/SDL.h>

typedef struct {
   // buffer size for audio input
   int buffsize;
   // buffer for audio input
   float* buffer;
   //
   // SDL audio device information
   SDL_AudioSpec wanteddev;
   SDL_AudioSpec audiodevice;
   SDL_AudioDeviceID devid_in;
   //
} audio_IO_t;

#endif
