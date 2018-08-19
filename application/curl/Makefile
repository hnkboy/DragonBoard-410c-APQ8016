CC = gcc
#CC = arm-linux-gcc
curlin = -I ./libcurl/include
curllib = -L ./libcurl/lib
cjsonlin = -I ./cJSON/include
cjsonlib = -L ./cJSON/lib
LIBS = -lcurl -lcjson -lasound -pthread -lrt
INCLUDE_PATH = $(curlin) $(alsain) $(cjsonin)
LIB_PATH = $(curllib) $(alsalib) $(cjsonlib)
ALSA_PATH = ./alsa
all: main.o getinmemory.o json.o puart.o plisten.o prevpkt.o pwork.o pvoice.o voiceclient.o queue.o  postcallback main_alsa 
	$(CC) main.o plisten.o pwork.o getinmemory.o postcallback.o md5.o json.o voiceclient.o ./alsa/playback.o ./alsa/playback_wav.o puart.o pvoice.o prevpkt.o queue.o -o main $(LIBS) $(INCLUDE_PATH) $(LIB_PATH)
main.o: main.c getinmemory.c json.c postcallback.c
	$(CC) -c main.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
puart.o: puart.c puart.h 
	$(CC) -c puart.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
plisten.o: plisten.c  plisten.h
	$(CC) -c plisten.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
prevpkt.o: prevpkt.c  prevpkt.h
	$(CC) -c prevpkt.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
pwork.o: pwork.c  pwork.h
	$(CC) -c pwork.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
pvoice.o: pvoice.c  pvoice.h
	$(CC) -c pvoice.c -I ./alsa $(INCLUDE_PATH) $(LIB_PATH)
queue.o: queue.c  queue.h
	$(CC) -c queue.c  $(INCLUDE_PATH) $(LIB_PATH)
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
