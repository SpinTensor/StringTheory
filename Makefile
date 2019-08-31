CC= gcc
CCFLAGS = -O3 -Wall -g \
	  -fmax-errors=3 -Wextra -fopenmp

INCFFTW = -I$$HOME/include/
LIBFFTW = -Wl,-Bstatic -L$$HOME/lib/ -lfftw3 -Wl,-Bdynamic -lpthread

LIBS = -lSDL2 -lm $(LIBFFTW)

all: AudioRecording.x

AudioRecording.x: AudioRecording.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBS)

AudioRecording.o: AudioRecording.c
	$(CC) $(CCFLAGS) $(INCFFTW) -c $<

.PHONY: clean veryclean

clean:
	rm -f *.o *.mod *.MOD

cleaner: clean
	rm -f *.x

