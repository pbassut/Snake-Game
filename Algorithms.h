/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#ifndef ALGORITHM_H
#define ALGORITHM_H

int qsort_helper_by_score(const void *e1, const void *e2);
bool check_collision(int collision_type);
BITMAP *new_food();

#endif
