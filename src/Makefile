CFLAGS = "-g "
all:hlite

hlite:hlite.o fcgi.o liteutil.o

fcgi.o:
	gcc  -g -c ./mod/fcgi.c  -I./include/ -O0

liteutil.o:
	gcc  -g -c ./misc/liteutil.c -I../include/ -I./include/ -O0

hlite.o:
	gcc  -g -c ./core/hlite.c -I./include/ -O0

test.o:
	gcc -g -c ./misc/test.c -I./include/  -O0

test:test.o
	cc liteutil.o test.o -o ./test -O0

go:
	rm *.o test ./hlite;make ;make test;./hlite -f ../etc/demo.conf


clean:
	rm *.o test ./hlite

run:
	./hlite -f ../etc/demo.conf

val:
	valgrind -v --track-origins=yes --tool=memcheck  --show-reachable=yes --leak-check=full ./hlite -f ../etc/demo.conf  

curl:
	curl -i http://localhost:1970/
.PHONY:clean

