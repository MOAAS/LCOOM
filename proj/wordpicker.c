#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include "wordpicker.h"

char* word_list[100];

static uint16_t word_count = 0;

static Bitmap* wordbox_bmp;
static Layer* background = NULL;
static char* word;
static bool* revealed_letters;
static uint16_t num_revealed_letters = 0;
static uint16_t word_size = 0;

static uint16_t wordbox_X = 145;
static uint16_t wordbox_Y = 50;
static uint16_t wordbox_width;
static uint16_t wordbox_height;

void loadDictionary() {
	char * filename = "home/lcom/labs/proj/palavras/palavras.txt";
	FILE * fp = fopen(filename, "r");
	if (fp == NULL) return;
	char c;
    char string[20] = "";
	while(1) {
        c = fgetc(fp);
		if(c == ' ' || c == '\n' || c == '\r' || c == EOF) {
            if (strlen(string) != 0) {
                word_list[word_count] = malloc(strlen(string) + 1);
                strcpy(word_list[word_count], string);
                strcpy(string, "");
                word_count++;
            }
            if (c == EOF)
                break;
		}
		else strncat(string , &c, 1);
	}
	fclose(fp); 
	return;
}

void word_pick_start(Layer* bg, char* solution, bool isDrawing) {
    background = bg;
    // Draw the box
    wordbox_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/wordbox.bmp");
    layer_draw_image(bg, wordbox_bmp, wordbox_X, wordbox_Y);
    wordbox_height = wordbox_bmp->bitmapInfoHeader.height;
    wordbox_width = wordbox_bmp->bitmapInfoHeader.width;
    // Getting a random word and moving it to current_word
    word_size = strlen(solution);
    word = malloc(word_size + 1);
    strcpy(word, solution);
    // Inicializar o bool array
    revealed_letters = malloc(word_size * sizeof(bool));
    num_revealed_letters = 0;
    for (int i = 0; i < word_size; i++) {
        revealed_letters[i] = false;
    }
    // Desenha: mostra a palavra
    if (isDrawing)
        draw_word(background, word, 2, 0, wordbox_X + (wordbox_width - word_size * 32) / 2 , wordbox_Y + wordbox_height / 2 - 16);
    // Nao desenha: Nao mostra
    else for (int i = 0; i < word_size; i++)
        draw_char(background, '_', 2, wordbox_X + (wordbox_width - word_size * 64) / 2 + i * 64, wordbox_Y + wordbox_height / 2 - 8);
}

void word_pick_end() {
    free(word);
    free(revealed_letters);
    word_size = 0;
    num_revealed_letters = 0;
    layer_draw_image(background, wordbox_bmp, wordbox_X, wordbox_Y);
    background = NULL;
}

char* get_random_word() {
    return word_list[rand() % word_count];
}

void reveal_letter() {
    int letter_no = rand() % word_size;
    if (revealed_letters[letter_no] == true || num_revealed_letters > word_size / 2)
        return;
    else {
        revealed_letters[letter_no] = true;
        num_revealed_letters++;
        draw_char(background, word[letter_no], 2, wordbox_X + (wordbox_width - word_size * 64) / 2 + letter_no * 64, wordbox_Y + wordbox_height / 2 - 16);
    }
}

bool verify_guess(char* guess) {
    if (strlen(guess) != strlen(word))
        return false;
    for (int i = 0; guess[i] != '\0'; i++) {
        if (tolower((int)guess[i]) != tolower((int)word[i]))
            return false;
    }
    return true;
}
