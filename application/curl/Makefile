CC = gcc
LIBS = -lcurl -lcjson -lasound -pthread
#INCLUDE_PATH = -I ./alsa 
ALSA_PATH = ./alsa
all: main.o getinmemory.o json.o postcallback main_alsa
	$(CC) main.o getinmemory.o postcallback.o md5.o json.o ./alsa/playback.o ./alsa/playback_wav.o -o main $(LIBS) $(INCLUDE_PATH)
main.o: main.c getinmemory.c json.c postcallback.c
	$(CC) -c main.c -I ./alsa
postcallback: postcallback.c md5.c
	$(CC) -c postcallback.c -I ./
	$(CC) -c md5.c
getinmemory.o: getinmemory.c json.c
	$(CC) -c getinmemory.c -I ./
json.o: json.c json.h
	$(CC) -c json.c -I ./cJSON
main_alsa:
	make -C $(ALSA_PATH)

clean: 
	rm ./*.o
	rm ./alsa/*.o
