CC = gcc

CFLAGS = -O0 -g -Wall -Werror -I.
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer
DEFINES = -Wall -DDEBUG -DJOYSTICK
DEFINES += -DSOUND_ENABLED
DEFINES += -DSAVE
OUTPUT = possum

SOURCES = src/main.c WRAPPER/PC_SDL2.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm src/*.o
	rm ${OUTPUT}
