CC = gcc
CFLAGS = -g
LIBS = -lasound
all:lplay lrecord

lplay: lplay.o sndwav_common.o wav_parser.o
	$(CC) $(CFLAGS) lplay.o sndwav_common.o wav_parser.o -o lplay $(LIBS)
lplay.o: lplay.c  sndwav_common.h wav_parser.h
	$(CC) $(CFLAGS) -c lplay.c  


lrecord: lrecord.o sndwav_common.o wav_parser.o
	$(CC) $(CFLAGS) lrecord.o sndwav_common.o wav_parser.o -o lrecord $(LIBS)
lrecord.o: lrecord.c sndwav_common.h  wav_parser.h
	$(CC) $(CFLAGS) -c lrecord.c 


sndwav_common.o: sndwav_common.c sndwav_common.h
	$(CC) $(CFLAGS) -c sndwav_common.c 


wav_parser.o: wav_parser.c wav_parser.h
	$(CC) $(CFLAGS) -c wav_parser.c


clean:
	rm lplay lrecord *.o
