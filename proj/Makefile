PROG=proj 

SRCS = proj.c uart.c uart_protocol.c mouse.c keyboard.c timer.c video.c interrupts.c queue.c vbe.c projeto.c bitmap.c layer.c canvas.c bucketqueue.c sprite.c event.c textbox.c clock.c wordpicker.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
