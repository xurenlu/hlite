all:hlite
lite:hlite.o hliteutil.o fcgi.o
	gcc -O0 -o hlite hlite.o hliteutil.o fcgi.o -lm 
hlite.o:hlite.c
	gcc -c -g -O0 lite.c
hliteutil.o:hliteutil.c
	gcc -c -g -O0 hliteutil.c

clean:
	rm *.o lite
run:
	./lite -f ./demo.conf
valg:
	valgrind -v --leak-check=full --show-reachable=yes ./lite -f ./demo.conf  
pkg:
	cp *.c *.h *.conf Makefile Changelog ./litehttp/
	tar -czf litehttp.tar.gz ./litehttp/
load:
	ab -n 100 -c 20 http://localhost:8080/4.cgi?id=go
