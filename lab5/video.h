#include "vbe.h"

struct color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};


void* video_init(uint16_t mode);

void video_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void video_draw_point(uint16_t x, uint16_t y, uint16_t thickness, uint32_t color);

void vg_draw_image(char* map, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

uint32_t vg_compose_RGB(uint8_t R, uint8_t G, uint8_t B);
struct color vg_decompose_RGB(uint32_t RGB);


unsigned int video_get_hres();
unsigned int video_get_vres();
unsigned int video_get_bits_pp();
bool vg_mode_is_indexed();
vbe_mode_info_t vg_get_mode();

void video_draw_line(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness);
void video_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void video_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

void video_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void video_draw_square(uint16_t x, uint16_t y, uint16_t side_len, uint32_t color);
void video_draw_circle (uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
void video_draw_circumference(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

void video_set_background(uint32_t color);

