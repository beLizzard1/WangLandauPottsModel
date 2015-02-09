CC=g++
CFLAGS=-O0 -std=gnu++11 -Wall -ggdb -pedantic 
LDFLAGS=-lm -lmgl -lconfig++
INCLUDES=-I/usr/local/include -I.

BinaryName=potts.app


OBJS=main.o potts.o utilityfunctions.o
all: $(OBJS) $(BinaryName)


%.o:        %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(BinaryName):
	$(CC) -o $(BinaryName) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJS)

clean:
	rm -rf *.o $(BinaryName)
