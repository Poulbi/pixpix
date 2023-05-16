PREFIX = /usr/local

LIBS = -lImlib2
SRC = pixelate.c

all: pixelate

pixelate: pixelate.c
	cc ${SRC} -o $@ -s ${LIBS}

clean:
	rm -f config.h

install: all
	install -Dm755 pixelate ${PREFIX}/bin/pixelate

uninstall:
	rm -rf ${PREFIX}/bin/pixelate

.PHONY: all clean install uninstall
