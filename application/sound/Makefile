CC = gcc
all: main_alsa
main_alsa: main.o playback.o playback_wav.o
	$(CC) -o $@ main.o playback.o playback_wav.o -lasound -pthread
playback.o: playback.c playback.h
	$(CC) -c -o $@ playback.c
playback_wav.o: playback_wav.c playback_wav.h
	$(CC) -c -o $@ playback_wav.c
main.0: main.c playback.o playback_wav.o
	$(CC) -c -o $@ main.c
clean:
	rm ./*.o 
