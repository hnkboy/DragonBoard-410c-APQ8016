#spawn-fcgi -a 127.0.0.1 -p 8089 -f /etc/nginx/sbin/test
#spawn-fcgi -a 127.0.0.1 -p 8089 -f /home/pi/DragonBoard-410c-APQ8016/application/curl/fastcgi/fcgi_read
#gcc a.c -g -o test  -I ../fcgi-2.4.0/include/ ../fcgi-2.4.0/libfcgi/.libs/libfcgi.a
#netstat -apn | grep 8089

##fcgi-write 只作为测试
#export LD_LIBRARY_PATH=/home/pi/fcgi-2.4.0/libfcgi/.libs
