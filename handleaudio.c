#include <stdlib.h>
#include <SDL2/SDL.h>

#include "fftw3.h"

#include "mp_constants.h"
#include "handleaudio.h"

audio_t init_audio(int buffersize){
   audio_t audiodata;

   // set audio buffer size
   audiodata.buffsize = buffersize;
   audiodata.audiobuffer = (float*)malloc(audiodata.buffsize*sizeof(float));
   // fill with zeros
   for (int i=0; i<audiodata.buffsize; i++){
      audiodata.audiobuffer[i] = 0.0;
   }

   // wanted audio input devide specifications
   SDL_zero(audiodata.wanteddev);
   audiodata.wanteddev.freq = 41000;                 // samples per second
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

   // Prepare fourier transform
   // is twice as big due to zero padding for non periodic fft
   audiodata.fft_insize = 2*audiodata.buffsize;
   audiodata.fft_indata = (double*)fftw_malloc(audiodata.fft_insize*sizeof(double));
   for (int i=audiodata.fft_insize/2; i<audiodata.fft_insize; i++){
      audiodata.fft_indata[i] = 0.0;
   }

   // is half as long as the input data array, but takes same amount of memory
   // as the output data is of complex type
   audiodata.fft_outsize = audiodata.fft_insize/2+1;
   audiodata.fft_outdata = (fftw_complex*)fftw_malloc(audiodata.fft_outsize*sizeof(fftw_complex));
   for (int i=0; i<audiodata.fft_outsize; i++){
      audiodata.fft_outdata[i][0] = 0.0;
      audiodata.fft_outdata[i][1] = 0.0;
   }

   // creating the fftw_plan
   audiodata.fftw_plan = 
      fftw_plan_dft_r2c_1d(audiodata.fft_insize,  //number of elements
                           audiodata.fft_indata,  //input data
                           audiodata.fft_outdata, //output data
                           FFTW_ESTIMATE);        //FFTW algorithm

   SDL_PauseAudioDevice(audiodata.devid_in, SDL_FALSE);
   return audiodata;
}

void get_audio_data(audio_t audiodata){
   // fill audio buffer
   SDL_DequeueAudio(audiodata.devid_in,                // device id
                    audiodata.audiobuffer,             // buffer
                    audiodata.buffsize*sizeof(float)); // number of bytes to obtain from audio input device
   SDL_ClearQueuedAudio(audiodata.devid_in); // clear pending audio data to avoid memory problems

   // filter and fourier transform data
   double s = twopi/((double)(audiodata.buffsize));
   for (int i=0; i<audiodata.buffsize; i++){
      // apply a han-filter window
      double w = 0.5 - 0.5*cos(s*(double)i);
      audiodata.fft_indata[i] = w * audiodata.audiobuffer[i];
   }

   fftw_execute(audiodata.fftw_plan);

   return ;

}

void free_audio(audio_t* audiodata){

   // stop the audio device
   SDL_PauseAudioDevice(audiodata->devid_in, SDL_TRUE);
   SDL_ClearQueuedAudio(audiodata->devid_in); // clear pending audio data to avoid memory problems
   SDL_CloseAudioDevice(audiodata->devid_in);

   // clear device specifications
   SDL_zero(audiodata->wanteddev);
   SDL_zero(audiodata->audiodevice);
   audiodata->devid_in = 0;

   // free audio buffer
   free(audiodata->audiobuffer);
   audiodata->buffsize = 0;

   // clear fft data
   fftw_destroy_plan(audiodata->fftw_plan);
   fftw_free(audiodata->fft_indata);
   audiodata->fft_insize = 0;
   fftw_free(audiodata->fft_outdata);
   audiodata->fft_outsize = 0;

   return;
}
