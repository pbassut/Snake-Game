/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

inline bool check_hscore(int points);
bool load_scores();
bool save_scores();
void show_hscore_list(char *name);
void set_hscore();

#endif
