CC= gcc
CCFLAGS = -O3 -Wall -g \
	  -fmax-errors=3 -Wextra

INCFFTW = -I$$HOME/include/
LIBFFTW = -Wl,-Bstatic -L$$HOME/lib/ -lfftw3 -Wl,-Bdynamic -lpthread
LIBFFTW = -L$$HOME/lib/ -lfftw3

INCSDL = 
LIBSDL = -lSDL2

INCGTK = `pkg-config --cflags gtk+-3.0`
LIBGTK = `pkg-config --libs gtk+-3.0` -export-dynamic

INCFLAGS = $(INCFFTW) $(INCSDL) $(INCGTK)
LIBS = -lm $(LIBFFTW) $(LIBSDL) $(LIBGTK) -export-dynamic

all: AudioRecording.x StringTheory.x

StringTheory.x: StringTheory.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

StringTheory.o: StringTheory.c StringTheory.glade
	$(CC) $(CCFLAGS) $(INCFLAGS) -c $<

AudioRecording.x: AudioRecording.o audio_IO.o freq_estimator.o fft.o window_functions.o mp_constants.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

AudioRecording.o: AudioRecording.c audio_IO.h fft.h freq_estimator.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

audio_IO.o: audio_IO.c
	$(CC) $(CCFLAGS) -c $<

freq_estimator.o: freq_estimator.c fft_t.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

fft.o: fft.c window_functions.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

window_functions.o: window_functions.c mp_constants.h
	$(CC) $(CCFLAGS) -c $<

mp_constants.o: mp_constants.c
	$(CC) $(CCFLAGS) -c $<

.PHONY: clean veryclean

clean:
	rm -f *.o

distclean: clean
	rm -f *.x

