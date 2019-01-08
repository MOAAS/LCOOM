#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include "wordpicker.h"

// word box
static const uint8_t FONT_SCALE = 2;
static const uint8_t SPACE_SCALE = 2;
static uint16_t wordbox_X = 145;
static uint16_t wordbox_Y = 50;
static uint16_t wordbox_xMid;
static uint16_t wordbox_yMid;
static uint16_t wordbox_width;
static uint16_t wordbox_height;


extern Bitmap* clock_bmp;
extern Bitmap* textbox2_bmp;
extern Bitmap* textbox6_bmp;

static Layer* background = NULL;

static char* word_list[250];
static uint16_t word_count = 0;

static uint16_t numRoundsDone = 0;
static uint16_t maxRounds = 6;

static char* word;
static uint16_t word_size = 0;
static char * used_words[50];

static bool* revealed_letters;
static uint16_t num_revealed_letters = 0;

static int score = 0;
static int highscore = 0;
static char * file_path;

void loadDictionary(char* path) {
	FILE * fp = fopen(path, "r");
	if (fp == NULL) {
        printf("File not found! %s\n", path);
        return;
    }
	char c;
    char string[20] = "";
	while(1) {
        c = fgetc(fp);
		if(c == '\n' || c == '\r' || c == EOF) {
            if (strlen(string) != 0) {
                word_list[word_count] = malloc(strlen(string) + 1);
                strcpy(word_list[word_count], string);
                strcpy(string, "");
                printf("size = %d, word = %s,  \n", strlen(word_list[word_count]), word_list[word_count]);
                word_count++;
            }
            if (c == EOF)
                break;
		}
		else strncat(string , &c, 1);
	}
	fclose(fp); 
    printf("Successfully loaded %d words. \n", word_count);
	return;
}

void wordgame_draw_hidden_word() {
    char* hidden_word = malloc(word_size * sizeof(char) + 1);
    for (int i = 0; i < word_size; i++) {
        if (word[i] != ' ')
            hidden_word[i] = '_';
        else hidden_word[i] = ' ';
    }
    hidden_word[word_size] = '\0';
    draw_word(background, hidden_word, wordbox_xMid, wordbox_yMid + 8, FONT_SCALE, SPACE_SCALE, CenterAlign);        
    free(hidden_word);
}

void wordgame_start_round(Layer* bg, char* solution, bool isDrawing) {
    background = bg;
    // Draw the box
    layer_draw_image(bg, textbox2_bmp, wordbox_X, wordbox_Y);
    wordbox_height = textbox2_bmp->bitmapInfoHeader.height;
    wordbox_width = textbox2_bmp->bitmapInfoHeader.width;
    wordbox_xMid = wordbox_X + wordbox_width / 2;
    wordbox_yMid = wordbox_Y + wordbox_height / 2 + 8;
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
    if (isDrawing)
        draw_word(background, word, wordbox_xMid, wordbox_yMid, FONT_SCALE, 0, CenterAlign);        
    else wordgame_draw_hidden_word();
    char round_counter[20];
    sprintf(round_counter, "%d/%d", (int)floor(numRoundsDone / 2) + 1, maxRounds / 2);
    draw_word(background, round_counter, 75, 100, 2, 0, CenterAlign);
    wordgame_create_clock();
}

void wordgame_end_round(int points) {
    wordgame_destroy_clock();
    free(revealed_letters);
    word_size = 0;
    used_words[numRoundsDone] = word;    
    num_revealed_letters = 0;
    numRoundsDone++;
    score += points;
}

void wordgame_set_rounds(uint16_t num_rounds) {
    if (num_rounds == 0 || num_rounds > 20)
        return;
    maxRounds = 2 * num_rounds;
}

bool is_wordgame_over() {
    return numRoundsDone >= maxRounds || numRoundsDone == 0;
}

void reset_wordgame() {
    for (int i = 0; i < numRoundsDone; i++)
        free(used_words[i]);
    score = 0;
    numRoundsDone = 0;
}

int wordgame_get_score() {
    return score;
}

char* get_random_word() {
    for (int i = 0; i < 10; i++) {
        unsigned rand_num = rand() % word_count;
        bool repeated_word = false;
        for (int j = 0; j < numRoundsDone; j++) {
            if (strcmp(word_list[rand_num], used_words[j]) == 0) {
                repeated_word = true;
                break;
            }
        }
        if (repeated_word)
            continue;
        else return word_list[rand_num];
    }
    return word_list[rand() % word_count];
}

char* get_solution() {
    return word;
}

void reveal_letter() {
    int letter_no = rand() % word_size;
    if (revealed_letters[letter_no] == true || num_revealed_letters >= word_size / 2)
        return;
    else {
        // n adianta tentar perceber mas é assim.
        int16_t xDisp_from_middle = 16 * (FONT_SCALE + SPACE_SCALE) * (letter_no - (word_size - 1) / 2.0);
        revealed_letters[letter_no] = true;
        num_revealed_letters++;
        draw_char(background, word[letter_no], wordbox_xMid + xDisp_from_middle, wordbox_yMid, FONT_SCALE, CenterAlign); 
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

void wordgame_showstats() {
    char score_str1[100];
    char score_str2[100];
    char highscore_str[100];
    if (score < highscore) {
        sprintf(highscore_str, "Highscore: %d", highscore);
    }
    else {
        highscore = score;
        sprintf(highscore_str, "New highscore: %d!", highscore);
        saveWordGameHighscore();
    }
    sprintf(score_str1, "You scored");
    sprintf(score_str2, "%d points", score);
    layer_draw_image(background, textbox6_bmp, 60, 250); // 860x100 image
    draw_word(background, score_str1, 500, 300, 3, 0, CenterAlign);
    tickdelay(micros_to_ticks(800000)); // 0.8 s
    draw_word(background, score_str2, 500, 400, 3, 0, CenterAlign);
    tickdelay(micros_to_ticks(800000)); // 0.8 s
    draw_word(background, highscore_str, 500, 500, 2, 0, CenterAlign);

}


void loadWordGameHighscore(char* path) {
    file_path = path;
    FILE *filePtr;
    filePtr = fopen(file_path, "rb");
    if (filePtr == NULL) {
        printf("wordgame file not found, highscore set to 0. File: %s \n", path);
        highscore = 0;
        return;
    }
    char temp[100];
    fscanf(filePtr, "%s %d", temp, &highscore);
    fclose(filePtr);
}

void saveWordGameHighscore() {
    FILE *filePtr = fopen(file_path, "w");
    if (filePtr == NULL) {
        printf("couldn't save highscore, path: %s \n", file_path);
        return;
    }
    fprintf(filePtr, "Highscore: %d", highscore);
    fclose(filePtr);
}

// Clock!

static uint16_t initial_time_left = 90;

static TextBox* clock_box;
static uint16_t time_left = 0;
static Layer* clock_layer = NULL;

void wordgame_create_clock() {
    clock_layer = create_layer(12, 150, 128, 128);
    clock_box = create_textbox(clock_layer, clock_bmp, 51, 66, 2, CenterAlign);
    time_left = initial_time_left;
    textbox_write_int(clock_box, time_left);
}

void wordgame_destroy_clock() {
    if (clock_box != NULL)
        destroy_textbox(clock_box);
    clock_layer = NULL;
    clock_box = NULL;
    time_left = 0;
}

uint16_t wordgame_get_time_left() {
    return time_left;
}

bool wordgame_time_up() {
    return time_left == 0;
}

void wordgame_tick_clock() {
    if (time_left == 0 || clock_box == NULL)
        return;
    time_left--;
    textbox_clear(clock_box);
    if (time_left < 10) {
        clock_box->cursorX_init = 80;
        clock_box->cursorX = 80;
    }
    textbox_write_int(clock_box, time_left);
}

void wordgame_set_clock(uint8_t t_left) {
    time_left = t_left + 1;
    wordgame_tick_clock();
}
