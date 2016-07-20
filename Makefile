CC = gcc
CFLAGS = -Wall -lpulse
LFLAGS = $(shell pkg-config --cflags --libs libpulse-simple)
FILES = fir.c pulseaudio_test.c
OUT_EXE = out

build: $(FILES)
	$(CC) $(CFLAGS) -o $(OUT_EXE) $(FILES) $(LFLAGS)

clean:
	rm -f *.o core out

