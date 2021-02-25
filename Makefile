# Makefile for Bible reader program using classes
# Use GNU c++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

all: biblereader

# Ref Object
Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

# Verse Object
Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

# Bible Object
Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp

# Main Program source
biblereader.o : Ref.h Verse.h Bible.h biblereader.cpp
	$(CC) $(CFLAGS) -c biblereader.cpp

# Build the executable
biblereader: Ref.o Verse.o Bible.o biblereader.o
	$(CC) $(CFLAGS) -o biblereader Ref.o Verse.o Bible.o biblereader.o

# "make clean" will clean up by removing intermediate files
clean:
	rm -f *.o biblereader core
