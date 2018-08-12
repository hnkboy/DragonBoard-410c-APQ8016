in = -I /home/jiali/Documents/FriendlyARM/toolschain/4.4.3/include/
lib = -L /home/jiali/Documents/FriendlyARM/toolschain/4.4.3/lib/
CC = arm-linux-gcc $(in) $(lib)
#CC = arm-linux-gcc -static $(lib) $(in)
#CC = cc
all: main_alsa
main_alsa: main.o playback.o playback_wav.o
	$(CC) -o $@ main.o playback.o playback_wav.o -lasound -pthread
#	$(CC) -o $@ main.o playback.o playback_wav.o -lasound
playback.o: playback.c playback.h
	$(CC) -c -o $@ playback.c
playback_wav.o: playback_wav.c playback_wav.h
	$(CC) -c -o $@ playback_wav.c
main.o: main.c playback.o playback_wav.o
	$(CC) -c -o $@ main.c
clean:
	rm ./*.o 
