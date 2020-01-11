#include <stdlib.h>
#include <SDL2/SDL.h>

#include "audio_IO.h"

audio_IO_t init_audio(int requests_per_second){
   audio_IO_t audiodata;

   // wanted audio input devide specifications
   SDL_zero(audiodata.wanteddev);
   audiodata.wanteddev.freq = 48000;                 // samples per second
   audiodata.wanteddev.format = AUDIO_F32;           // audio_format is float
   audiodata.wanteddev.channels = 1;                 // number of channels
   audiodata.wanteddev.samples = audiodata.buffsize; // buffersize to write in
   audiodata.wanteddev.callback = NULL;              // need callback? no!

   // Get audio input device
   SDL_zero(audiodata.audiodevice);
   audiodata.devid_in = 
      SDL_OpenAudioDevice(NULL,                      // Identify audio device with string. no
                          SDL_TRUE,                  // Is a capture device. yes
                          &(audiodata.wanteddev),    // wanted audio specifications
                          &(audiodata.audiodevice),  // delivered audio specifications
                          SDL_AUDIO_ALLOW_FORMAT_CHANGE);

   // set audio buffer size
   audiodata.buffsize = audiodata.audiodevice.freq / requests_per_second;
   audiodata.buffer = (float*)malloc(audiodata.buffsize*sizeof(float));
   // fill with zeros
   for (int i=0; i<audiodata.buffsize; i++){
      audiodata.buffer[i] = 0.0;
   }

   SDL_PauseAudioDevice(audiodata.devid_in, SDL_FALSE);
   return audiodata;
}

void get_audio_data(audio_IO_t audiodata){
   // fill audio buffer
   SDL_DequeueAudio(audiodata.devid_in,                // device id
                    audiodata.buffer,             // buffer
                    audiodata.buffsize*sizeof(float)); // number of bytes to obtain from audio input device
   SDL_ClearQueuedAudio(audiodata.devid_in); // clear pending audio data to avoid memory problems

   return ;
}

void free_audio(audio_IO_t* audiodata){

   // stop the audio device
   SDL_PauseAudioDevice(audiodata->devid_in, SDL_TRUE);
   SDL_ClearQueuedAudio(audiodata->devid_in); // clear pending audio data to avoid memory problems
   SDL_CloseAudioDevice(audiodata->devid_in);

   // clear device specifications
   SDL_zero(audiodata->wanteddev);
   SDL_zero(audiodata->audiodevice);
   audiodata->devid_in = 0;

   // free audio buffer
   free(audiodata->buffer);
   audiodata->buffsize = 0;

   return;
}
