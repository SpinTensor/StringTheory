CC= gcc
CCFLAGS = -O3 -Wall -g \
	  -fmax-errors=3 -Wextra

INCFFTW = -I$$HOME/include/
LIBFFTW = -Wl,-Bstatic -L$$HOME/lib/ -lfftw3 -Wl,-Bdynamic -lpthread
LIBFFTW = -L$$HOME/lib/ -lfftw3

INCSDL = 
LIBSDL = -lSDL2

INCGTK = $$(pkg-config --cflags gtk+-3.0)
LIBGTK = $$(pkg-config --libs gtk+-3.0) -export-dynamic

INCFLAGS = $(INCFFTW) $(INCSDL) $(INCGTK)
LIBS = -lm $(LIBFFTW) $(LIBSDL) $(LIBGTK) -export-dynamic

all: AudioRecording.x StringTheory.x AudioVisualization.x

StringTheory.x: StringTheory.o audio_IO.o freq_estimator.o fft.o window_functions.o mp_constants.o notes.o note_names.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

StringTheory.o: StringTheory.c audio_IO.h audio_IO_t.h fft.h freq_estimator.h notes.h StringTheory.glade
	$(CC) $(CCFLAGS) $(INCFLAGS) -c $<

AudioRecording.x: AudioRecording.o audio_IO.o freq_estimator.o fft.o window_functions.o mp_constants.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

AudioRecording.o: AudioRecording.c audio_IO.h audio_IO_t.h fft.h freq_estimator.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

AudioVisualization.x: AudioVisualization.o audio_IO.o fft.o window_functions.o mp_constants.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

AudioVisualization.o: AudioVisualization.c audio_IO.h audio_IO_t.h fft.h
	$(CC) $(CCFLAGS) $(INCFLAGS) -c $<

audio_IO.o: audio_IO.c audio_IO_t.h
	$(CC) $(CCFLAGS) -c $<

freq_estimator.o: freq_estimator.c fft_t.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

fft.o: fft.c window_functions.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

window_functions.o: window_functions.c mp_constants.h
	$(CC) $(CCFLAGS) -c $<

mp_constants.o: mp_constants.c
	$(CC) $(CCFLAGS) -c $<

notes.o: notes.c note_names.h equal_tempered_ratios.h just_tempered_ratios.h
	$(CC) $(CFLAGS) -c $<

note_names.o: note_names.c
	$(CC) $(CFLAGS) -c $<

equal_tempered_ratios.h: generate_equal_tempering_ratios.x
	(./$< > $@)

generate_equal_tempering_ratios.x: generate_equal_tempering_ratios.c
	$(CC) $(CFLAGS) -o $@ $< -lm

just_tempered_ratios.h: generate_just_tempering_ratios.x
	(./$< > $@)

generate_just_tempering_ratios.x: generate_just_tempering_ratios.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean veryclean

clean:
	rm -f *.o
	-rm generate_equal_tempering_ratios.x generate_just_tempering_ratios.x
	-rm equal_tempered_ratios.h just_tempered_ratios.h

distclean: clean
	rm -f *.x

