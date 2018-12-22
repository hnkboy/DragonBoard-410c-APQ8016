#!/bin/bash 
echo "depoll th fast-cgi\n"
#export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):/home/pi/fcgi-2.4.0/libfcgi/.libs
spawn-fcgi -a 127.0.0.1 -p 8089 -f /home/pi/DragonBoard-410c-APQ8016/application/curl/fastcgi/fcgi_read
#netstat -apn | grep 8089
