CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

all: render_tone render_song render_echo

render_tone: render_tone.o wave.o io.o
	$(CC) -o render_tone render_tone.o wave.o io.o -lm

render_song: render_song.o wave.o io.o
	$(CC) -o render_song render_song.o wave.o io.o -lm

render_echo: render_echo.o wave.o io.o
	$(CC) -o render_echo render_echo.o wave.o io.o -lm

wave.o: wave.c io.h wave.h
	$(CC) $(CFLAGS) -c wave.c

render_tone.o: render_tone.c  io.h wave.h
	$(CC) $(CFLAGS) -c render_tone.c

render_song.o: render_song.c io.h wave.h
	$(CC) $(CFLAGS) -c render_song.c

render_echo.o: render_echo.c io.h wave.h
	$(CC) $(CFLAGS) -c render_echo.c

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c


clean:
	rm -f *.o render_tone
	rm -f *.o render_song
	rm -f *.o render_echo
