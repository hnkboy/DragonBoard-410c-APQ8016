#CC = gcc
alsain = -I /home/jiali/Documents/FriendlyARM/toolschain/4.4.3/include/
alsalib = -L /home/jiali/Documents/FriendlyARM/toolschain/4.4.3/lib/
cjsonin = -I /home/jiali/nfsdir/home/jiali/Documents/curl/cJSON/include
cjsonlib = -L /home/jiali/nfsdir/home/jiali/Documents/curl/cJSON/lib
curlin = -I /home/jiali/nfsdir/home/jiali/Documents/curl/libcurl/include
curllib = -L /home/jiali/nfsdir/home/jiali/Documents/curl/libcurl/lib
alsain = -I /home/jiali/nfsdir/home/jiali/Documents/curl/alsa
CC = arm-linux-gcc
LIBS = -lcurl -lcjson -lasound -pthread
#LIBS =   -lasound -pthread -lcjson
INCLUDE_PATH = $(curlin) $(alsain) $(cjsonin) $(alsain)
LIB_PATH = $(curllib) $(alsalib) $(cjsonlib)
ALSA_PATH = /home/jiali/nfsdir/home/jiali/Documents/curl/alsa
all: voiceserver.o  main_alsa 
	$(CC) voiceserver.o  $(ALSA_PATH)/playback.o $(ALSA_PATH)/playback_wav.o  -o voiceserver $(LIBS) $(INCLUDE_PATH) $(LIB_PATH)
voiceserver.o: voiceserver.c
	$(CC) -c voiceserver.c $(INCLUDE_PATH) $(LIB_PATH)
main_alsa:
	make -C $(ALSA_PATH)
clean: 
	rm ./*.o
	rm $(ALSA_PATH)/*.o
