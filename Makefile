LIBS = -lImlib2
SRC = pixelate.c

all: pixelate

pixelate: pixelate.c
	cc ${SRC} -o $@ -s ${LIBS}

clean:
	rm -f config.h

install: all
	install -Dm755 pixelate /usr/local/bin/pixelate

uninstall:
	rm -rf /usr/local/bin/pixelate
