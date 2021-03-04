# Makefile for Bible reader program using classes
# Use GNU c++ compiler with C++11 standard
CC= g++
CFLAGS= -g -std=c++11

all: testreader

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
testreader.o : Ref.h Verse.h Bible.h testreader.cpp
	$(CC) $(CFLAGS) -c testreader.cpp

# Build the executable
testreader: Ref.o Verse.o Bible.o testreader.o
	$(CC) $(CFLAGS) -o testreader Ref.o Verse.o Bible.o testreader.o

# "make clean" will clean up by removing intermediate files
clean:
	rm -f *.o titlereader core
