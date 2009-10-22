all:hlite

hlite:hlite.o fcgi.o liteutil.o

fcgi.o:
	gcc  -c ./mod/fcgi.c  -I./include/

liteutil.o:
	gcc  -c ./misc/liteutil.c -I../include/ -I./include/

hlite.o:
	gcc  -c ./core/hlite.c -I./include/

test.o:
	gcc -c ./misc/test.c -I./include/ 

test:test.o
	cc liteutil.o test.o -o ./test

go:
	rm *.o test ./hlite;make ;make test;./hlite -f ../etc/demo.conf


clean:
	rm *.o test ./hlite
