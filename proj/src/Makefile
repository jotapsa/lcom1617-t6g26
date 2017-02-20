#Makefile for project lcom1617-t6g26

COMPILER_TYPE= gnu
CC= gcc

PROG= tron
SRCS= tron.c timer.c mouse.c kbd.c events.c players.c mp_local.c rtc.c menu.c video_gr.c vbe.c bitmap.c level.c scoreboard.c singleplayer.c

CCFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
