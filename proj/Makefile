PROG=proj 

SRCS = proj.c uart.c uart_protocol.c uart_wordgame.c mouse.c keyboard.c timer.c rtc.c video.c interrupts.c queue.c vbe.c projeto.c bitmap.c bitmaps.c layer.c canvas.c sprite.c event.c textbox.c wordpicker.c snake.c flappy.c emote.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
