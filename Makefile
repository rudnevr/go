CC=gcc 
CFLAGS=-Wall
main: main.o up_down.o hook.o

clean:
	rm -f main main.o up_down.o
