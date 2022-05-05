CC=g++
CFLAGS=-Wall -O3 -std=c++14
BINS=Tarea
all: clean Tarea

Tarea:
	$(CC) $(CFLAGS) -o Tarea main.cpp

clean:
	@echo " [CLN] Removing binary files"
	@rm -f $(BINS)
