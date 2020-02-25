#ifndef AUDIO_IO_H
#define AUDIO_IO_H

#include "audio_IO_t.h"

void init_audio(audio_IO_t *audiodata, int requests_per_second);

void free_audio(audio_IO_t* audiodata);

#endif
