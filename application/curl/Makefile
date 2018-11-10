CC = gcc
#CC = arm-linux-gcc
curlin = -I ./libcurl/include
curllib = -L ./libcurl/lib
cjsonlin = -I ./cJSON/include
cjsonlib = -L ./cJSON/lib
LIBS = -lcurl -lcjson -lasound -pthread -lrt -lmad
INCLUDE_PATH = $(curlin) $(alsain) $(cjsonin)
LIB_PATH = $(curllib) $(alsalib) $(cjsonlib)
ALSA_PATH = ./alsa
REST_TTS_PATH = ./rest-api-tts
DEV_PATH = ./device
all: main.o getinmemory.o json.o plisten.o prevpkt.o pwork.o pvoice.o  queue.o  postcallback dev rest_tts 
	$(CC) main.o plisten.o pwork.o getinmemory.o postcallback.o md5.o json.o pvoice.o prevpkt.o queue.o alsaplay.o alsaplay_wav.o ./device/*.o ./rest-api-tts/*.o  -o Hali $(LIBS) $(INCLUDE_PATH) $(LIB_PATH)
main.o: main.c getinmemory.c json.c postcallback.c
	$(CC) -c main.c  $(INCLUDE_PATH) $(LIB_PATH)
puart.o: puart.c puart.h 
	$(CC) -c puart.c  $(INCLUDE_PATH) $(LIB_PATH)
plisten.o: plisten.c  plisten.h
	$(CC) -c plisten.c  $(INCLUDE_PATH) $(LIB_PATH)
prevpkt.o: prevpkt.c  prevpkt.h
	$(CC) -c prevpkt.c  $(INCLUDE_PATH) $(LIB_PATH)
pwork.o: pwork.c  pwork.h 
	$(CC) -c pwork.c  $(INCLUDE_PATH) $(LIB_PATH)
pvoice.o: pvoice.c  pvoice.h  alsaplay.c  alsaplay.h alsaplay_wav.c alsaplay_wav.h
	$(CC) -c pvoice.c  $(INCLUDE_PATH) $(LIB_PATH)
	$(CC) -c alsaplay.c $(INCLUDE_PATH) $(LIB_PATH)
	$(CC) -c alsaplay_wav.c $(INCLUDE_PATH) $(LIB_PATH)
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
dev:  
	make -C $(DEV_PATH)
main_alsa:
	make -C $(ALSA_PATH)
rest_tts:
	make -C $(REST_TTS_PATH)
clean: 
	rm ./*.o
	#rm ./*.wav
	#rm ./*.txt
	rm $(REST_TTS_PATH)/*.o
	rm ./device/*.o
	#rm $(REST_TTS_PATH)/*.wav
