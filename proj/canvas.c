#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "canvas.h"

static Canvas* canvas = NULL;
void create_canvas(Layer* layer, uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax, uint32_t color) {
    canvas = malloc(sizeof(Canvas));
    canvas->layer = layer;
    canvas->xMin = xMin;
    canvas->xMax = xMax;
    canvas->yMin = yMin;
    canvas->yMax = yMax;
    canvas->width = xMax - xMin;
    canvas->height = yMax - yMin;
    canvas_set_color(color);
    canvas_set_outline(BLACK);
}

void destroy_canvas() {
    if (canvas != NULL)
        return;
    free(canvas);
    canvas = NULL;    
}

void canvas_draw_line(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness) {
    int16_t dx = xf - x0, dy = yf - y0;
    if (dx == 0 && dy == 0) { // Single left-click
       canvas_draw_circle(x0, y0, thickness, color);
       return;
    }
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
       canvas_draw_circle(round(x), round(y), thickness, color);
    }
}

void canvas_draw_line2(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness) {
    int16_t dx = xf - x0, dy = yf - y0;
    if (dx == 0 && dy == 0) { // Single left-click
       canvas_draw_square(x0 - thickness, y0 - thickness,  2 * thickness, color);
       return;
    }
    unsigned int steps;
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else steps = abs(dy);
    float x = x0 - thickness, y = y0 - thickness;
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;
    for(unsigned int i = 0; i < steps; i++) {
       x = x + xInc;
       y = y + yInc;
       canvas_draw_square(round(x), round(y), 2 * thickness, color);
    }
}

void canvas_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
    // Bresenham’s Algorithm
    int D = 3 - 2 * (int)radius;
    int P = 0, Q = radius;
    while (P <= Q) {
        canvas_draw_hline(x - P, y + Q, 2 * P, color);
        canvas_draw_hline(x - P, y - Q, 2 * P, color);
        canvas_draw_hline(x - Q, y + P, 2 * Q, color);
        canvas_draw_hline(x - Q, y - P, 2 * Q, color);
        P++;
        if (D <= 0)
            D = D + 4*P + 6;
        else {
            Q--;
            D = D + 4 * (P-Q) +10;
        }
    }
}

void canvas_draw_circumference(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
    // Bresenham’s Algorithm
    int D = 3 - 2 * (int)radius;
    int P = 0, Q = radius;
    while (P <= Q) {
        canvas_draw_pixel(x + P, y + Q, color);
        canvas_draw_pixel(x - P, y + Q, color);
        canvas_draw_pixel(x + P, y - Q, color);
        canvas_draw_pixel(x - P, y - Q, color);
        canvas_draw_pixel(x + Q, y + P, color);
        canvas_draw_pixel(x - Q, y + P, color);
        canvas_draw_pixel(x + Q, y - P, color);
        canvas_draw_pixel(x - Q, y - P, color);
        P++;
        if (D <= 0)
            D = D + 4*P + 6;
        else {
            Q--;
            D = D + 4 * (P-Q) +10;
        }
    }
}

void canvas_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (canvas->layer == NULL || !is_inside_canvas(x, y))
        return;
    draw_on_layer(canvas->layer, x, y, color);
}

// usado pelo draw circle, importante estar otimizado
void canvas_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    if (canvas->layer == NULL)
        return;
    char* layer_addr = calc_address(canvas->layer->map, x, y, canvas->layer->width);
    char* video_addr = calc_address(vg_get_video_mem(), x, y, vg_get_hres());
    uint8_t bytes_pp = vg_get_bytes_pp();
    for (size_t i = 0; i < len; i++, x++) {
        if (is_inside_canvas(x, y)) {
            vg_insert(layer_addr, color);
            if (is_top_layer(canvas->layer, x, y))
                vg_insert(video_addr, color);
        }
        layer_addr += bytes_pp;
        video_addr += bytes_pp;
    }
}

void canvas_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (size_t i = 0; i < len; i++)
        canvas_draw_pixel(x, y++, color);
}

void canvas_draw_rectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint32_t color) {
    if (canvas->layer == NULL)
        return;
        
    char* layer_addr = calc_address(canvas->layer->map, x0, y0, canvas->layer->width);
    char* video_addr = calc_address(vg_get_video_mem(), x0, y0, vg_get_hres());
    uint8_t bytes_pp = vg_get_bytes_pp();
    for (int i = 0, y = y0; i < height; i++, y++) {
        for (int j = 0, x = x0; j < width; j++, x++) {
            if (is_inside_canvas(x, y)) {
                vg_insert(layer_addr, color);
                if (is_top_layer(canvas->layer, x, y))
                    vg_insert(video_addr, color);
            }            
            layer_addr += bytes_pp;
            video_addr += bytes_pp;
        }
        layer_addr = layer_addr + (canvas->layer->width - canvas->width) * bytes_pp;
        video_addr = video_addr + (vg_get_hres() - canvas->width) * bytes_pp;
    }

    
}

void canvas_draw_rectangle_outline(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t thickness, uint32_t color) {

    for(size_t i = 0; i < thickness; i++) {
        canvas_draw_hline(x, y + i, width, color);
        canvas_draw_hline(x, y - i + height, width, color);
        canvas_draw_vline(x + i, y, height, color);
        canvas_draw_vline(x - i + width, y, height, color);
    }
}

void canvas_draw_square(uint16_t x, uint16_t y, uint16_t side_len, uint32_t color) {
    for (size_t i = 0; i < side_len; i++)
        canvas_draw_hline(x, y++, side_len, color);
}

void canvas_set_outline(uint32_t color) {
    uint16_t x = canvas->xMin - 5, y = canvas->yMin - 5;
    uint16_t width = canvas->width + 10, height = canvas->height + 10;
    if (!is_within_bounds(canvas->layer, x, y) || !is_within_bounds(canvas->layer, x + width - 1, y + height - 1))
        return;
    for(size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < height; j++) { // Vertical
            draw_on_layer(canvas->layer, x + i, y + j, color);
            draw_on_layer(canvas->layer, x + width - i - 1, y + j, color);
        }
        for (size_t j = 0; j < width; j++) { // Horizontal
            draw_on_layer(canvas->layer, x + j, y + i, 0);
            draw_on_layer(canvas->layer, x + j, y + height - i - 1, color);
        }
    }
}

void canvas_draw_image(Bitmap* bitmap) {
    layer_draw_image(canvas->layer, bitmap, canvas->xMin, canvas->yMin);
}

void canvas_set_color(uint32_t color) {
    canvas_draw_rectangle(canvas->xMin, canvas->yMin, canvas->width, canvas->height, color);
}

uint32_t rainbow(uint32_t old_color) {
    struct color color = vg_decompose_RGB(old_color);
    if (color.G < 255 && color.B == 0 && color.R == 255) color.G += 3;    
    else if (color.R > 0 && color.G == 255 && color.B == 0 ) color.R -= 3;
    else if (color.B < 255 && color.R == 0 && color.G == 255) color.B += 3;
    else if (color.G > 0 && color.R == 0 && color.B == 255) color.G -= 3;
    else if (color.R < 255 && color.G == 0 && color.B == 255) color.R += 3;
    else if (color.B > 0 && color.R == 255 && color.G == 0) color.B -= 3;
    else color.B-= 3;
    return vg_compose_RGB(color.R, color.G, color.B);
}

bool is_inside_canvas(uint16_t x , uint16_t y) {
    return x >= canvas->xMin && y >= canvas->yMin && x < canvas->xMax && y < canvas->yMax;
}

bool is_past_hor_bounds(uint16_t x) {
    return x > canvas->xMax || x < canvas->xMin;
}

bool is_past_ver_bounds(uint16_t y) {
    return y > canvas->yMax || y < canvas->yMin;
}


char* canvas_get_map() {
    uint8_t bytes_per_pixel = vg_get_bytes_pp();
    char* canvas_map = malloc(canvas->width * canvas->height * bytes_per_pixel);
    char* layer_map = calc_address(canvas->layer->map, canvas->xMin, canvas->yMin, canvas->layer->width); 
    char* canvas_curr = canvas_map;
    for (int i = 0; i < canvas->height; i++) {
        memcpy(canvas_curr, layer_map, canvas->width * bytes_per_pixel);
        canvas_curr += canvas->width * bytes_per_pixel;         
        layer_map += canvas->layer->width * bytes_per_pixel;   
    }
    return canvas_map;
}

int canvas_get_height() {
    return canvas->height;
}

int canvas_get_width() {
    return canvas->width;
}    

void bucket_tool_rec(int x, int y, uint32_t cor_balde, uint32_t cor_inicial) {
    int minX = x - 1, maxX = x;
    uint8_t bytes_pp = vg_get_bytes_pp();

    char* layer_addr1 = calc_address(canvas->layer->map, x - 1, y, canvas->layer->width);
    char* layer_addr2 = layer_addr1 + bytes_pp;

    while (minX >= canvas->xMin && vg_retrieve(layer_addr1) == cor_inicial) {
        vg_insert(layer_addr1, cor_balde);

        if (is_top_layer(canvas->layer, minX, y))
            vg_draw_pixel(minX, y, cor_balde);

        layer_addr1 -= bytes_pp;        
        minX--;
    }
    minX++;

    while (maxX < canvas->xMax && vg_retrieve(layer_addr2) == cor_inicial) {
        vg_insert(layer_addr2, cor_balde);

        if (is_top_layer(canvas->layer, maxX, y))
            vg_draw_pixel(maxX, y, cor_balde);

        layer_addr2 += bytes_pp;        
        maxX++;
    }
    maxX--;

    layer_addr1 = calc_address(canvas->layer->map, minX, y - 1, canvas->layer->width);
    layer_addr2 = calc_address(canvas->layer->map, minX, y + 1, canvas->layer->width);
    for (int i = minX; i < maxX; i++) {                     
        if (y > canvas->yMin && vg_retrieve(layer_addr1) == cor_inicial) 
            bucket_tool_rec(i, y - 1, cor_balde, cor_inicial);          
        if (y < canvas->yMax - 1 && vg_retrieve(layer_addr2) == cor_inicial)
            bucket_tool_rec(i, y + 1, cor_balde, cor_inicial);  
        layer_addr1 += bytes_pp;            
        layer_addr2 += bytes_pp;       
    }
}

void bucket_tool(uint16_t x, uint16_t y, uint32_t cor_balde) {
    uint32_t cor_inicial = layer_get_pixel(canvas->layer, x, y);
    if (cor_inicial == cor_balde || !is_inside_canvas(x, y))
        return;
    bucket_tool_rec(x,y,cor_balde, cor_inicial);
    return;

    // para a maneira clean mas n fast
    uint8_t bytes_pp = vg_get_bytes_pp();
    char* layer_map = canvas->layer->map + (canvas->xMin + canvas->yMin * canvas->layer->width) * bytes_pp;
    char* video_mem = (char*)vg_get_video_mem() + (canvas->xMin + canvas->yMin * vg_get_hres()) * bytes_pp;
    for (int y = canvas->yMin; y < canvas->yMax; y++) {
        for (int x = canvas->xMin; x < canvas->xMax; x++) {
            if (is_top_layer(canvas->layer, x, y))
                vg_insert(video_mem, vg_retrieve(layer_map));
            layer_map += bytes_pp;
            video_mem += bytes_pp;
        }
        layer_map = layer_map + (canvas->layer->width - canvas->width) * bytes_pp;
        video_mem = video_mem + (vg_get_hres() - canvas->width) * bytes_pp;
    }
    //

}
