.phony: all clean

all:
	gcc -I include/ src/vdb.c -o vdb
