CC = gcc
#alsain = -I /home/jiali/Documents/FriendlyARM/toolschain/4.4.3/include/
#alsalib = -L /home/jiali/Documents/FriendlyARM/toolschain/4.4.3/lib/
curlin = -I ./libcurl/include
curllib = -L ./libcurl/lib
#CC = arm-linux-gcc
LIBS = -lcurl -lcjson -lasound -pthread
LIBS = -lcurl -lcjson -lasound -pthread
#LIBS =   -lasound -pthread -lcjson
INCLUDE_PATH = $(curlin) $(alsain) $(cjsonin)
LIB_PATH = $(curllib) $(alsalib) $(cjsonlib)
ALSA_PATH = ./alsa
all: main.o getinmemory.o json.o puart.o plisten.o pwork.o pvoice.o voiceclient.o postcallback main_alsa 
	$(CC) main.o plisten.o pwork.o getinmemory.o postcallback.o md5.o json.o voiceclient.o ./alsa/playback.o ./alsa/playback_wav.o puart.o pvoice.o -o main $(LIBS) $(INCLUDE_PATH) $(LIB_PATH)
main.o: main.c getinmemory.c json.c postcallback.c
	$(CC) -c main.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
puart.o: puart.c puart.h 
	$(CC) -c puart.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
plisten.o: plisten.c  plisten.h
	$(CC) -c plisten.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
pwork.o: pwork.c  pwork.h
	$(CC) -c pwork.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
pvoice.o: pvoice.c  pvoice.h
	$(CC) -c pvoice.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
postcallback: postcallback.c md5.c
	$(CC) -c postcallback.c -I ./ $(INCLUDE_PATH) $(LIB_PATH)
	$(CC) -c md5.c
getinmemory.o: getinmemory.c json.c json.h
	$(CC) -c getinmemory.c -I ./ -I ./cJSON $(INCLUDE_PATH) $(LIB_PATH)
json.o: json.c json.h
	$(CC) -c json.c -I ./cJSON
voiceclient.o: voiceclient.c voiceclient.h
	$(CC) -c voiceclient.c
main_alsa:
	make -C $(ALSA_PATH)
clean: 
	rm ./*.o
	rm ./alsa/*.o
