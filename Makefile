CC=g++
CFLAGS=-Wall -O3 -std=c++14
BINS=Tarea HuffmanC
all: clean Tarea HuffmanC

HuffmanC:
	$(CC) $(CFLAGS) -o HuffmanC HuffmanCanonico.cpp 
Tarea:
	$(CC) $(CFLAGS) -o Tarea main.cpp

clean:
	@echo " [CLN] Removing binary files"
	@rm -f $(BINS)
