CC= gcc
CCFLAGS = -O3 -Wall -g \
	  -fmax-errors=3 -Wextra

INCFFTW = -I$$HOME/include/
LIBFFTW = -Wl,-Bstatic -L$$HOME/lib/ -lfftw3 -Wl,-Bdynamic -lpthread
LIBFFTW = -L$$HOME/lib/ -lfftw3

LIBS = -lSDL2 -lm $(LIBFFTW)

all: AudioRecording.x

AudioRecording.x: AudioRecording.o handleaudio.o mp_constants.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

AudioRecording.o: AudioRecording.c handleaudio.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

handleaudio.o: handleaudio.c mp_constants.h
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

mp_constants.o: mp_constants.c
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

.PHONY: clean veryclean

clean:
	rm -f *.o

distclean: clean
	rm -f *.x

