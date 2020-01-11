#ifndef AUDIO_IO_H
#define AUDIO_IO_H

#include "audio_IO_t.h"

audio_IO_t init_audio(int requests_per_second);

void get_audio_data(audio_IO_t audiodata);

void free_audio(audio_IO_t* audiodata);

#endif
