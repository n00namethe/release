.PHONY: all clean

all: pipe

pipe.o: pipe.c
		gcc -c -o build/pipe.o pipe.c

pipe: pipe.o
		gcc -o build/pipe build/pipe.o

clean:
		rm -f build/*
