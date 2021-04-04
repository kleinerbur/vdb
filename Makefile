.phony: all clean

all:
	gcc -w  -I include/ src/vdb.c -o vdb
