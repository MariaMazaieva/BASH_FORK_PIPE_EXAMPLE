CC=gcc
CXX=g++

CXXFLAGS = -Wall $(EXTRA_CFLAGS)
CFLAGS= -Wall $(EXTRA_CFLAGS)
DEPS = nd.h nsd.h
SRC = nsd_main.c nsd.c nd.c
OBJ = $(SRC:.c=.o)

all: forkpipe nsd

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

forkpipe: forkpipe.cpp 
	$(CXX) -o forkpipe forkpipe.cpp $(CXXFLAGS)

nsd: $(OBJ)
	$(CC) -o nsd $(OBJ) $(CFLAGS)


# %: %.o
# 	$(CC) $< -o $@ $(CFLAGS)
clean:
	rm -f *.o forkpipe nsd 