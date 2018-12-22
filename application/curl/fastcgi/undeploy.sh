#!/bin/bash 
echo "undepoll th fast-cgi\n"
#export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):/home/pi/fcgi-2.4.0/libfcgi/.libs
#spawn-fcgi -a 127.0.0.1 -p 8089 -f /home/pi/DragonBoard-410c-APQ8016/application/curl/fastcgi/fcgi_read
#netstat -apn | grep 8089
procedure_name=/home/pi/DragonBoard-410c-APQ8016/application/curl/fastcgi/fcgi_read
#ps -ef | grep $procedure_name | grep -v grep | awk '{print $2}' | xargs kill -9
PROCESS=`ps -ef | grep $procedure_name | grep -v grep | grep -v PPID | awk '{ print $2}'`
for i in $PROCESS
do
    echo "Kill the $1 process [ $i ]"
    kill -9 $i
done
