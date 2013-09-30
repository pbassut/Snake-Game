/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File: main.h
**/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "Images.h"
#include "Fonts.h"
#include "Sounds.h"

#define WINDOW_TITLE "Snake Game by Patrick Bassut"
#define VIDEO_CARD    GFX_AUTODETECT_WINDOWED
#define MAX_X		800
#define MAX_Y		600
#define V_MAX_X		0
#define V_MAX_Y		0
#define COLOR_BITS  32
#define DIGI_CARD   DIGI_AUTODETECT
#define MIDI_CARD   MIDI_AUTODETECT

#define WHITE makecol(255, 255, 255)
#define BLACK makecol(0, 0, 0)
#define PINK makecol(255, 0, 255)
#define RED makecol(255, 0, 0)
#define BLUE makecol(0, 0, 255)

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define LEFT_BORDER 18
#define RIGHT_BORDER 766
#define TOP_BORDER -13
#define BOTTOM_BORDER 577

#define WALL_COLLISION 0
#define ITSELF_COLLISION 1
#define FOOD_COLLISION 2

#define INIT_SPEED (0.3)
#define START_TAILS 2


typedef enum{false, true}bool;

typedef struct _POS{
    int x;
    int y;
}Pos;

typedef struct _config{
    int resolutionx;
    int resolutiony;
    bool window_mode;
    bool changed;
    bool music;
    bool sound;
    int volume;
}tConfig;

typedef struct _snake{
    Pos pos;
    int prev_snake_pos_x[400]; // Váriavel usada para 'guardar' os rastros X da snake.
    int prev_snake_pos_y[400]; // Váriavel usada para 'guardar' os rastros Y da snake.
    int tails_number;
    int direction;
    float speed;
}tSnake;

typedef struct _game{
    tConfig cur_cfg;
    Pos possible_food[24];
    Pos food;
    int food_type;
    int points;
    float speed_food_time;
    Pos spr;
    bool is_paused;
    bool is_playing;
}tGame;

typedef struct _Score
{
  int player_score;
  char player_name[11];
}Score;

extern tSnake player;
extern tGame game;
extern DATAFILE *img_datafile;
extern DATAFILE *snd_datafile;
extern DATAFILE *fnt_datafile;
extern Score score_table[11];

int qsort_helper_by_score(const void *e1, const void *e2);
bool check_collision(int collision_type);
BITMAP *new_food();
inline void close_game();
void game_play();
void snake_crashed(int points);
void game_introduction();
void read_string(char *str_to, int x, int y, int size);
void fadein(BITMAP *bmp_orig, int speed, int until);
void fadeout(int speed, int until);
inline bool check_hscore(int points);
bool load_scores();
bool save_scores();
void show_hscore_list(char *name);
void set_hscore();
bool init_allegro();
bool init_game();
bool init_settings();
int main_menu();
void option_menu();
void apply_settings(tConfig new_config);

#endif
