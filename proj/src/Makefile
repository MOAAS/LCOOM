PROG=proj 

SRCS = proj.c uart.c uart_protocol.c uart_wordgame.c mouse.c keyboard.c timer.c rtc.c video.c interrupts.c queue.c vbe.c pengoo.c bitmap.c bitmaps.c layer.c canvas.c sprite.c event.c textbox.c clock.c wordpicker.c snake.c flappy.c emote.c game_state.c game_info.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
