all:hlite

hlite:hlite.o fcgi.o liteutil.o

fcgi.o:
	gcc  -c ./mod/fcgi.c  -I./include/ -O0

liteutil.o:
	gcc  -c ./misc/liteutil.c -I../include/ -I./include/ -O0

hlite.o:
	gcc  -c ./core/hlite.c -I./include/ -O0

test.o:
	gcc -c ./misc/test.c -I./include/  -O0

test:test.o
	cc liteutil.o test.o -o ./test -O0

go:
	rm *.o test ./hlite;make ;make test;./hlite -f ../etc/demo.conf


clean:
	rm *.o test ./hlite

run:
	./hlite -f ../etc/demo.conf
