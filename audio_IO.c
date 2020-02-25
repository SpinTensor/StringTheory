#include <stdlib.h>
#include <SDL2/SDL.h>

#include "audio_IO.h"

// Callback function to copy the updated audio input stream to the audio buffer
void get_audio_data_callback(void *userdata, Uint8* stream, int len) {

   audio_IO_t audiodata = *(audio_IO_t*) userdata;
   // get number of floats to be written to the buffers end
   int nfloats = len / sizeof(float);
   // move buffer "len" bytes to the left to make space for the new data
   for (int i=0; i<audiodata.buffsize-nfloats; i++) {
      audiodata.buffer[i] = audiodata.buffer[i+nfloats];
   }

   // copy the new audio data from the byte stream to the end of the buffer
   float *fstream = (float*) stream;
   for (int i=0; i<nfloats; i++) {
      audiodata.buffer[i+audiodata.buffsize-nfloats] = fstream[i];
   }

   return;
}

void init_audio(audio_IO_t *audiodata, int requests_per_second){

   // wanted audio input devide specifications
   SDL_zero(audiodata->wanteddev);
   audiodata->wanteddev.freq = 44000;                 // samples per second
   audiodata->wanteddev.format = AUDIO_F32;           // audio_format is float
   audiodata->wanteddev.channels = 1;                 // number of channels
   audiodata->wanteddev.samples = 512;                // stream sample size
   audiodata->wanteddev.callback = get_audio_data_callback;  // need callback
   audiodata->wanteddev.userdata = audiodata;

   // Get audio input device
   SDL_zero(audiodata->audiodevice);
   audiodata->devid_in = 
      SDL_OpenAudioDevice(NULL,                      // Identify audio device with string. no
                          SDL_TRUE,                  // Is a capture device. yes
                          &(audiodata->wanteddev),    // wanted audio specifications
                          &(audiodata->audiodevice),  // delivered audio specifications
                          SDL_AUDIO_ALLOW_FORMAT_CHANGE);

   // set audio buffer size
   audiodata->buffsize = audiodata->audiodevice.freq / requests_per_second;
   audiodata->buffer = (float*)malloc(2*audiodata->buffsize*sizeof(float));
   // fill with zeros
   for (int i=0; i<audiodata->buffsize; i++){
      audiodata->buffer[i] = 0.0;
   }

   SDL_PauseAudioDevice(audiodata->devid_in, SDL_FALSE);
   return;
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
