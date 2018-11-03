cc = gcc
all: fcgi_read.c
	$(cc) fcgi_read.c -I /home/pi/fcgi-2.4.0/include/ /home/pi/fcgi-2.4.0/libfcgi/.libs/libfcgi.a -lpthread -o fcgi_read -lrt
#	$(cc) fcgi_write.c -I /home/pi/fcgi-2.4.0/include/ /home/pi/fcgi-2.4.0/libfcgi/.libs/libfcgi.a -lpthread -o fcgi_write -lrt
clean:
	rm ./*.o
