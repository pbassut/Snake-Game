

/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#ifndef GRAPH_FUNCS_H
#define GRAPH_FUNCS_H

void snake_crashed(int points);
void game_introduction();
void read_string(char *str_to, int x, int y, int size);
void fadein(BITMAP *bmp_orig, int speed, int until);
void fadeout(int speed, int until);

#endif
