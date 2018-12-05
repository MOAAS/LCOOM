PROG=lab5 

SRCS = lab5.c mouse.c keyboard.c timer.c video.c interrupts.c Sprite.c vbe.c

CPPFLAGS += -pedantic -DLAB5
CPPFLAGS += -D LAB5

DPADD += ${LIBLCF} ${LIBLM}
LDADD += -llcf -llm

.include <minix.lcom.mk>
