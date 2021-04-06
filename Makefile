.phony: all debug clean

SOURCES = src/vdb.c
DESTDIR = .

all:
	@gcc -w  -I include/ ${SOURCES} -o ${DESTDIR}/vdb

debug:
	@gcc -w  -ggdb3 -I include/ ${SOURCES} -o ${DESTDIR}/vdb

clean:
	@rm ${DESTDIR}/vdb
