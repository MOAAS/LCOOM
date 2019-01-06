#include <lcom/lcf.h>

#include "clock.h"

extern Bitmap* clockbig_bmp;

static TextBox* big_ben1 = NULL;
static TextBox* big_ben2 = NULL;

static Layer* big_ben_layer = NULL;

void create_big_ben() {
    big_ben_layer = create_layer(10, 10, 181, 181);
    big_ben1 = create_textbox(big_ben_layer, clockbig_bmp, 36, 75, 1, CenterAlign);
    big_ben2 = create_textbox(big_ben_layer, clockbig_bmp, 36, 106, 1, CenterAlign);
    update_big_ben(true);    
}

void destroy_big_ben() {
    free(big_ben2);
    destroy_textbox(big_ben1);
    big_ben1 = NULL;
    big_ben2 = NULL;
    big_ben_layer = NULL;
}

void update_big_ben(bool force) {
    char time1_str[100];
    char time2_str[100];
    Date d1 = rtc_get_date();
    if (force || d1.sec == 0) {
        sprintf(time1_str, "%02x/%02x/%02x", d1.day, d1.mth, d1.yer);
        sprintf(time2_str, "%02x:%02x:%02x", d1.hor, d1.min, d1.sec);
        textbox_clear(big_ben1);
        textbox_write(big_ben1, time1_str);
        textbox_clear(big_ben2);
        textbox_write(big_ben2, time2_str);
    }
    else {
        textbox_backspace(big_ben2);
        textbox_backspace(big_ben2);
        sprintf(time2_str, "%02x", d1.sec);
        textbox_write(big_ben2, time2_str);
    }
}
