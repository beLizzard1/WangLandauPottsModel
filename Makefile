CC=g++
CFLAGS=-O3 -std=gnu++11 -ggdb -pedantic -Wall -Wextra -Wformat=2 -Wmissing-declarations -Wstrict-overflow=5 -Wswitch-default
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
