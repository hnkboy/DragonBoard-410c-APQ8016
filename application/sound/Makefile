CC = gcc
all: main_alsa
main_alsa: main.o playback.o
	$(CC) -o $@ main.o playback.o -lasound -pthread
playback.o: playback.c playback.h
	$(CC) -c -o $@ playback.c
main.0: main.c playback.o
	$(CC) -c -o $@ main.c
clean:
	rm ./*.o 
