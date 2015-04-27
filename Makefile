CC=g++
CFLAGS=-O3 -std=gnu++11 -pedantic -Wall -Wextra
LDFLAGS=-lm -lconfig++ 
INCLUDES=-I/usr/local/include -I. -I websocketpp/ 

BinaryName=potts.app


OBJS=main.o potts.o metropolis.o wanglandau.o utilityfunctions.o
all: $(OBJS) $(BinaryName)


%.o:        %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(BinaryName):
	$(CC) -o $(BinaryName) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJS)

clean:
	rm -rf *.dat *.o $(BinaryName)
