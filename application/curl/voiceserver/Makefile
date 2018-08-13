CC = gcc
curlin = -I ../libcurl/include
curllib = -L ../libcurl/lib
alsain = -I ../alsa
#CC = arm-linux-gcc
LIBS = -lcurl -lcjson -lasound -pthread
#LIBS =   -lasound -pthread -lcjson
INCLUDE_PATH = $(curlin) $(alsain) $(cjsonin) $(alsain)
LIB_PATH = $(curllib) $(alsalib) $(cjsonlib)
ALSA_PATH =../alsa
all: voiceserver.o  main_alsa 
	$(CC) voiceserver.o  $(ALSA_PATH)/playback.o $(ALSA_PATH)/playback_wav.o  -o voiceserver $(LIBS) $(INCLUDE_PATH) $(LIB_PATH)
voiceserver.o: voiceserver.c
	$(CC) -c voiceserver.c $(INCLUDE_PATH) $(LIB_PATH)
main_alsa:
	make -C $(ALSA_PATH)
clean: 
	rm ./*.o
	rm $(ALSA_PATH)/*.o
