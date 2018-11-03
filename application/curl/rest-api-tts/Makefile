CC = gcc
all: tts
tts: ttsmain.o common.o token.o ttscurl.o
#tts: ttsmain.c common.o token.o ttscurl.o
#	$(CC) -o $@ ttsmain.c token.o common.o ttscurl.o -lcurl 
ttsmain.o: ttsmain.c ttsmain.h
	$(CC) -c -o $@ ttsmain.c
common.o: common.c common.h
	$(CC) -c -o $@ common.c
token.o: token.c token.h
	$(CC) -c -o $@ token.c
ttscurl.o: ttscurl.c ttscurl.h
	$(CC) -c -o $@ ttscurl.c
clean:
	rm ./*.o 

