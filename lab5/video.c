#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "video.h"

static void *video_mem;		    // Process (virtual) address to which VRAM is mapped 

static unsigned int h_res;	        // Horizontal resolution in pixels 
static unsigned int v_res;	        // Vertical resolution in pixels 
static unsigned int bits_per_pixel; // Number of VRAM bits per pixel
static vbe_mode_info_t vbe_mode;

void video_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= h_res || y >= v_res)
        return;
    unsigned int bytes_per_pixel = ceil(bits_per_pixel / 8.0);
    // Will store each byte in order
    char * pixel_ptr = (char *)video_mem + (x + h_res * y) * bytes_per_pixel;
    for (size_t offset = 0; offset < bytes_per_pixel; offset++, color = color >> 8) {
       *(pixel_ptr + offset) = color;
    }
    return;
}

uint32_t vg_compose_RGB(uint8_t R, uint8_t G, uint8_t B) {
    return (R << vbe_mode.RedFieldPosition) | (G << vbe_mode.GreenFieldPosition) | (B << vbe_mode.BlueFieldPosition);
    uint32_t color = 0;
    color |= B;
    color |= G << vbe_mode.BlueMaskSize;
    color |= R << (vbe_mode.BlueMaskSize + vbe_mode.GreenMaskSize);
    return color;
}

struct color vg_decompose_RGB(uint32_t RGB) {
    struct color color;
    color.B = RGB;
    color.G = RGB >> vbe_mode.BlueMaskSize;
    color.R = RGB >> (vbe_mode.BlueMaskSize + vbe_mode.GreenMaskSize);
    color.B = color.B & ((1 << vbe_mode.BlueMaskSize) - 1);
    color.G = color.G & ((1 << vbe_mode.GreenMaskSize) - 1);
    color.R = color.R & ((1 << vbe_mode.RedMaskSize) - 1);
    return color;

}

void video_draw_point(uint16_t x, uint16_t y, uint16_t thickness, uint32_t color) {
    video_draw_circle(x,y,thickness, color);
}

void video_draw_line(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness) {
    int16_t dx = xf - x0, dy = yf - y0;
    unsigned int steps;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else steps = abs(dy);
    float x = x0, y = y0;
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;
    for(unsigned int i = 0; i < steps; i++) {
       x = x + xInc;
       y = y + yInc;
       video_draw_point(round(x), round(y), thickness, color);
    }
}

void video_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (size_t i = 0; i < len; i++)
        video_draw_pixel(x++, y, color);    
}

void video_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (size_t i = 0; i < len; i++)
        video_draw_pixel(x, y++, color);    
}

void video_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (size_t i = 0; i < height; i++)
      video_draw_hline(x, y++, width, color);
}

void video_draw_square(uint16_t x, uint16_t y, uint16_t side_len, uint32_t color) {
    for (size_t i = 0; i < side_len; i++)
      video_draw_hline(x, y++, side_len, color);
}


void video_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
    // Bresenham’s Algorithm
    int D = 3 - 2 * (int)radius;
    int P = 0, Q = radius;
    while (P <= Q) {
        video_draw_hline(x - P, y + Q, 2 * P, color);
        video_draw_hline(x - P, y - Q, 2 * P, color);
        video_draw_hline(x - Q, y + P, 2 * Q, color);
        video_draw_hline(x - Q, y - P, 2 * Q, color);
        P++;
        if (D <= 0)
            D = D + 4*P + 6;
        else {
            Q--;
            D = D + 4 * (P-Q) +10;
        }
        video_draw_hline(x - P, y + Q, 2 * P, color);
        video_draw_hline(x - P, y - Q, 2 * P, color);
        video_draw_hline(x - Q, y + P, 2 * Q, color);
        video_draw_hline(x - Q, y - P, 2 * Q, color);
    }
}

void video_draw_circumference(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
    // Bresenham’s Algorithm
    int D = 3 - 2 * (int)radius;
    int P = 0, Q = radius;
    while (P <= Q) {
        video_draw_pixel(x + P, y + Q, color);
        video_draw_pixel(x - P, y + Q, color);
        video_draw_pixel(x + P, y - Q, color);
        video_draw_pixel(x - P, y - Q, color);
        video_draw_pixel(x + Q, y + P, color);
        video_draw_pixel(x - Q, y + P, color);
        video_draw_pixel(x + Q, y - P, color);
        video_draw_pixel(x - Q, y - P, color);
        P++;
        if (D <= 0)
            D = D + 4*P + 6;
        else {
            Q--;
            D = D + 4 * (P-Q) +10;
        }
        video_draw_pixel(x + P, y + Q, color);
        video_draw_pixel(x - P, y + Q, color);
        video_draw_pixel(x + P, y - Q, color);
        video_draw_pixel(x - P, y - Q, color);
        video_draw_pixel(x + Q, y + P, color);
        video_draw_pixel(x - Q, y + P, color);
        video_draw_pixel(x + Q, y - P, color);
        video_draw_pixel(x - Q, y - P, color);
    }
}

void video_set_background(uint32_t color) {
    video_draw_rectangle(0, 0, h_res, v_res, color);
}

void vg_draw_image(char* map, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
    for (int i = 0; i < width; i++)   {
        for (int j = 0; j < height; j++) {
            uint32_t color = map[j*width + i];
            if (color != 0)
                video_draw_pixel(x + i, y + j, color);
        }
    }
}

unsigned int video_get_bits_pp() {
    return bits_per_pixel;
}

unsigned int video_get_hres() {
    return h_res;
}

unsigned int video_get_vres() {
    return v_res;
}

bool vg_mode_is_indexed() {
    return bits_per_pixel == 8;
}

vbe_mode_info_t vg_get_mode() {
    return vbe_mode;
}


void* video_init(uint16_t mode) {
    while (lm_init(false) == NULL) {
        printf("Lm_init failed! Retrying... \n");
    }
	if (vbe_get_modeInfo(mode, &vbe_mode) != 0)
        return NULL;
        
	v_res = vbe_mode.YResolution;
	h_res = vbe_mode.XResolution;
	bits_per_pixel = vbe_mode.BitsPerPixel;
    unsigned int vram_base = vbe_mode.PhysBasePtr;  // VRAM's physical addresss 
    unsigned int vram_size = h_res * v_res * ceil(bits_per_pixel / 8.0); // VRAM's size

    int r;
    struct minix_mem_range mr;

    // Allow memory mapping
    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
       panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    // Map memory 
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
       panic("couldn't map video memory");

    struct reg86u reg;
    memset(&reg, 0, sizeof(reg));	// zero the structure
    reg.u.w.ax = VBE_SET_MODE;
    reg.u.w.bx = VBE_SET_LIN_BUF | mode; // set bit 14: linear framebuffer
    reg.u.b.intno = VBE_INT_VECTOR;
    if( sys_int86(&reg) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    if (reg.u.b.ah == VBE_FUNC_FAIL) {
		printf("Function call failed!\n");
		return NULL;
	}
	else if (reg.u.b.ah == VBE_FUNC_UNSUPPORTED) {
		printf("Function is not supported in current HW configuration!\n");
		return NULL;
	}
	else if (reg.u.b.ah == VBE_FUNC_INVALID) {
		printf("Function is invalid in current video mode!\n");
		return NULL;
	}
    return video_mem;
}


