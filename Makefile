LIBS = -lImlib2
SRC = pixelate.c

all: pixelate

config.h:
	cp config.def.h $@

pixelate: pixelate.c config.h
	cc ${SRC} -o $@ -s ${LIBS}

clean:
	rm config.h

install: all
	install -Dm755 pixelate /usr/local/bin/pixelate
