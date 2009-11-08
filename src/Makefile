CFLAGS = -g -O0 -I./include/
RM=rm -f 
BIN="./hlite" 
#OBJECTS="./core/hlite.o ./misc/fcgi.o ./misc/liteutil.o"
vpath %.h ./include
vpath %.c core
vpath %.c misc
vpath %.c mod


all:hlite

#hlite:$(OBJECTS)
hlite:hlite.o fcgi.o liteutil.o

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
curl:
	curl -i http://localhost:1970/
.PHONY:clean
include $(sources:.c=.d)
%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; 
	#sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	#rm -f $@.$$$$

