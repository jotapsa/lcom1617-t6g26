COMPILER_TYPE= gnu

CC=gcc

PROG=	lab5
SRCS=	lab5.c test5.c video_gr.c vbe.c timer.c test3.c read_xpm.c sprite.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=
BINDIR?= /usr/sbins

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
