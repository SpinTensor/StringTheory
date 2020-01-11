#ifndef HANDLEAUDIO_H
#define HANDLEAUDIO_H

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


audio_IO_t init_audio(int requests_per_second);

void get_audio_data(audio_IO_t audiodata);

void free_audio(audio_IO_t* audiodata);

#endif
