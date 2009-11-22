CFLAGS = -g -O0 -I./include/ -lpthread
RM=rm -f 
BIN="./hlite" 
LDFLAGS= -lpthread 
CC=gcc
#OBJECTS="./core/hlite.o ./misc/fcgi.o ./misc/liteutil.o"
vpath %.h ./include
vpath %.c kern
vpath %.c misc
vpath %.c mod


all:hlite test

#hlite:$(OBJECTS)
hlite:hlite.o fcgi.o liteutil.o
%.o:%.c %.h


test:test.o
	cc liteutil.o test.o -o ./test -O0
go:
	rm *.o test ./hlite;make ;make test;./hlite -f ../etc/demo.conf
clean:
	$(RM) *.o test ./hlite
run:
	$(BIN)  -f ../etc/demo.conf
val:
	valgrind -v --track-origins=yes --tool=memcheck  --show-reachable=yes --leak-check=full ./hlite -f ../etc/demo.conf  
mem:
	valgrind -v  --tool=memcheck  --leak-check=full ./hlite -f ../etc/demo.conf  
testmem:
	valgrind -v  --tool=memcheck  --leak-check=full ./test

curl:
	curl -i http://localhost:1973/

.PHONY:clean

load:
	rm /tmp/url73;echo "http://localhost:1973/wifi.sh" >> /tmp/url73; http_load -p 6 -f 500 /tmp/url73;
#rm /tmp/url73;echo "http://localhost:1973/" >> /tmp/url73;echo "http://localhost:1973/wifi.sh" >> /tmp/url73;echo "http://localhost:1973/js/" >> /tmp/url73; http_load -p 30 -f 6000 /tmp/url73;
ab:
	ab -n 1000 http://localhost:1973/ & ab -n 1000 http://localhost:1973/ & ab -n 1000 http://localhost:1973/ & ab -n 1000 http://localhost:1973/ &
