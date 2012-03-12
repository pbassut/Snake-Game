/**
This code was completely made by Patrick Bassut.
If you wanna make any change, please, contact-me at patrickbassut@hotmail.com.
Enjoy it!
**/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <string.h>
#include <time.h>
#include "images.h"
#include "Sounds.h"
#include "Fonts.h"

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

#define INIT_SPEED 200

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
    int speed;
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

tSnake player;
tGame game;
DATAFILE *img_datafile = NULL;
DATAFILE *snd_datafile = NULL;
DATAFILE *fnt_datafile = NULL;
Score score_table[11];

bool init_allegro();
bool init_game();
bool init_settings();
void game_play();
void game_introduction();
int main_menu();
void option_menu();
void snake_crashed();
bool check_collision(int collision_type);
void apply_settings(tConfig new_config);
inline BITMAP *new_food();

void fadein(BITMAP *bmp_orig, int speed, int until);
void fadeout(int speed, int until);
bool drag_effect(BITMAP *drag, int fx, int fy, int tx, int ty);
void read_string(char *str_to, int x, int y, int size);

int cal_curve(int i1, int x1, int y1, int i2, int x2, int y2);

inline void close_game();

inline bool check_hscore(int points);
bool load_scores();
bool save_scores();
void sort_scores();
void show_hscore_list(char *name);
void set_hscore();

#endif
