#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "bucketqueue.h"
#include "canvas.h"

Canvas* canvas = NULL;
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
    canvas_set_color(WHITE);
    canvas_set_outline(WHITE);
    free(canvas);
    canvas = NULL;    
}

void canvas_draw_line1(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness) {
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
    char* layer_addr = calc_address(canvas->layer->map, x, y, canvas->layer->width);
    char* video_addr = calc_address(vg_get_video_mem(), x, y, vg_get_hres());
    uint8_t bytes_pp = vg_get_bytes_pp();
    for (size_t i = 0; i < len; i++, x++) {
        if (canvas->layer != NULL && is_inside_canvas(x, y)) {
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
    char* layer_addr = calc_address(canvas->layer->map, x0, y0, canvas->layer->width);
    char* video_addr = calc_address(vg_get_video_mem(), x0, y0, vg_get_hres());
    uint8_t bytes_pp = vg_get_bytes_pp();
    for (int i = 0, y = y0; i < height; i++, y++) {
        for (int j = 0, x = x0; j < width; j++, x++) {
            if (canvas->layer != NULL && is_inside_canvas(x, y)) {
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

void canvas_set_color(uint32_t color) {
    canvas_draw_rectangle(canvas->xMin, canvas->yMin, canvas->width, canvas->height, color);
}

uint32_t rainbow() {
    static uint32_t R = 255, G = 0 , B = 0;
    if(G<255 && B==0 && R==255) G+=5;    
    else if(R>0 && G==255 && B==0 ) R-=5;
    else if(B<255 && R==0 && G == 255) B+=5;
    else if(G>0 && R == 0 && B == 255) G-=5;
    else if(R<255 && G==0 && B == 255) R+=5;
    else if(B>0 && R==255 && G==0) B-=5;
    return vg_compose_RGB(R, G, B);
} 

bool is_inside_canvas(uint16_t x , uint16_t y) {
    return x >= canvas->xMin && y >= canvas->yMin && x < canvas->xMax && y < canvas->yMax;
}


void bucket_tool(Sprite* cursor, uint32_t cor_inicial, uint32_t cor_balde) {
    if (cor_inicial == cor_balde || !is_inside_canvas(cursor->x, cursor->y))
        return;
    // Cria fila e coloca as posicoes iniciais nela
    //Queue* Q = create_queue();
    char* layer_map = canvas->layer->map + (cursor->x + cursor->y * canvas->layer->width) * vg_get_bytes_pp();
    queue_destroy();
    queue_push(cursor->x, cursor->y, layer_map);
    BucketQueuePoint P;
    uint8_t bytes_pp = vg_get_bytes_pp();
    uint32_t vertical_diff = vg_get_hres() * bytes_pp;
    while(!queue_isEmpty()) {
        // Trata as posicoes que estao a frente na fila
        P = queue_pop();
        // Se estiver a ser tratado um pixel valido, continua
        if(vg_retrieve(P.layer_address) == cor_inicial) {
            // Adiciona os 4 pixeis a volta a fila, se nao tiverem sido tratados ainda
            vg_insert(P.layer_address, cor_balde);
            P.layer_address -= bytes_pp;
            if(is_inside_canvas(P.x-1, P.y) && vg_retrieve(P.layer_address) != cor_balde) {
                queue_push(P.x-1, P.y, P.layer_address);
            }
            P.layer_address += 2 * bytes_pp;
            if(is_inside_canvas(P.x+1, P.y) && vg_retrieve(P.layer_address) != cor_balde) {
                queue_push(P.x+1, P.y, P.layer_address);
            }
            P.layer_address -= bytes_pp + vertical_diff;
            if(is_inside_canvas(P.x, P.y-1) && vg_retrieve(P.layer_address) != cor_balde) {
                queue_push(P.x, P.y-1, P.layer_address);
            }
            P.layer_address += 2 * vertical_diff;
            if(is_inside_canvas(P.x, P.y+1) && vg_retrieve(P.layer_address) != cor_balde) {
                queue_push(P.x, P.y+1, P.layer_address);
            }
        }
    }
    queue_destroy();
    layer_map = canvas->layer->map + (canvas->xMin + canvas->yMin * canvas->layer->width) * bytes_pp;
    char* video_mem = (char*)vg_get_video_mem() + (canvas->xMin + canvas->yMin * vg_get_hres()) * bytes_pp;
    for (int y = canvas->yMin; y < canvas->yMax; y++) {
        for (int x = canvas->xMin; x < canvas->xMax; x++) {
            vg_insert(video_mem, vg_retrieve(layer_map));
            layer_map += bytes_pp;
            video_mem += bytes_pp;
        }
        layer_map = layer_map + (canvas->layer->width - canvas->width) * bytes_pp;
        video_mem = video_mem + (vg_get_hres() - canvas->width) * bytes_pp;
    }
}

/*
void bucket_tool(uint16_t x0 , uint16_t y0, uint32_t cor_inicial, uint32_t cor_balde) {
    if (cor_inicial == cor_balde || !is_inside_canvas(x0, y0))
        return;
    // Cria fila e coloca as posicoes iniciais nela
    Queue* Q = create_queue();
    char* layer_map = canvas->layer->map + (x0 + y0 * canvas->layer->width) * vg_get_bytes_pp();
    char* video_mem = (char*)vg_get_video_mem() + (x0 + y0 * vg_get_hres()) * vg_get_bytes_pp();
    push(Q, x0, y0, layer_map, video_mem);
    Point p;
    uint32_t diff = vg_get_hres() * 3;
    x0 = y0 = 3;
    while(Q->front != NULL) {
        // Trata as posicoes que estao a frente na fila
        p = front(Q);
        pop(Q);
        if (!is_inside_canvas(p.x, p.y))
            continue;
        // Se estiver a ser tratado um pixel valido, continua
        if(vg_retrieve(p.layer_address) == cor_inicial) {
            // Adiciona os 4 pixeis a volta a fila, se nao tiverem sido tratados ainda
            vg_insert(p.layer_address, cor_balde);
            vg_insert(p.vg_address, cor_balde);
            if(vg_retrieve(p.layer_address - 3) != cor_balde) {
                push(Q, p.x-1, p.y, p.layer_address - 3, p.vg_address - 3);
            }
            if(vg_retrieve(p.layer_address + 3) != cor_balde) {
                push(Q, p.x+1, p.y, p.layer_address + 3, p.vg_address + 3);
            }
            if(vg_retrieve(p.layer_address - diff) != cor_balde) {
                push(Q, p.x, p.y-1, p.layer_address - diff, p.vg_address - diff);
            }
            if(vg_retrieve(p.layer_address + diff) != cor_balde) {
                push(Q, p.x, p.y+1, p.layer_address + diff, p.vg_address + diff);
            }
        }
    }
    destroy_queue(Q);
}
*/