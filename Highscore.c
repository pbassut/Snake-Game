/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "main.h"

inline bool check_hscore(int points)
{
    if(points > score_table[9].player_score)
        return true;
    else return false;
}

void set_hscore(int points)
{
    clear_keybuf();
    char name[10];
    FONT *fonte = (FONT*)fnt_datafile[COMICSANS].dat;
    rect(screen, SCREEN_W/3.23, SCREEN_H/2.13, SCREEN_W/1.46, SCREEN_H/1.89, WHITE); /// Draws the rectangle for the user input the name
    textout_ex(screen, fonte, "Your name: ", SCREEN_W/6.3, SCREEN_H/2.1, WHITE, -1);
    read_string(name, SCREEN_W/3, SCREEN_H/2, 10); /// get the player's name
    strcat(name, " "); ///Add some trash in the end of the name, so we can point it out later
    strcpy(score_table[10].player_name, name);
    score_table[10].player_score = points; /// Inserts the score in the last position of the SCORE_TABLE
    sort_scores(); /// Since we add another player in the last position of the score_table, we should sort it to get its proper position
    show_hscore_list(name); /// Show highscore list pointing out name
    return;
}

void show_hscore_list(char *name)
{
    fadeout(50, 256);
    if(!name)
    {
        name = malloc(sizeof(char));
        strcpy(name, "    ");
    }
    FONT *fonte = (FONT*)fnt_datafile[COMICSANS].dat;
    BITMAP *high_score = (BITMAP*)img_datafile[HIGH_SCORE].dat;

    fadein(high_score, 10, 120);

    register int i;
    for(i = 1; i < 10; i++)
    {
        char score[5];
        itoa(score_table[i-1].player_score, score, 10);
        if(!strcmp(score_table[i-1].player_name, name))
        {
            char aux[11]; /// shorter variable
            strcpy(aux, score_table[i-1].player_name);
            aux[strlen(aux)-1] = '\0';
            strcpy(score_table[i-1].player_name, aux);

            textout_ex(screen, fonte, score_table[i-1].player_name, 240, 50+(48*i), RED, -1);
            textout_ex(screen, fonte, score, 535, 50+(48*i), RED, -1);
        }
        else{
            textout_ex(screen, fonte, score_table[i-1].player_name, 240, 50+(48*i), WHITE, -1);
            textout_ex(screen, fonte, score, 535, 50+(48*i), WHITE, -1);
        }

    }
    textout_ex(screen, font, "Press any key to exit", 320, SCREEN_H-50, WHITE, -1);

    while(!keypressed());
}

void set_default_scores()
{
    strcpy(score_table[0].player_name, "Elephant");
    score_table[0].player_score = 500;
    strcpy(score_table[1].player_name, "Hippo");
    score_table[1].player_score = 400;
    strcpy(score_table[2].player_name, "Giraffe");
    score_table[2].player_score = 300;
    strcpy(score_table[3].player_name, "Zebra");
    score_table[3].player_score = 250;
    strcpy(score_table[4].player_name, "Lion");
    score_table[4].player_score = 200;
    strcpy(score_table[5].player_name, "Wombat");
    score_table[5].player_score = 150;
    strcpy(score_table[6].player_name, "Kangaroo");
    score_table[6].player_score = 100;
    strcpy(score_table[7].player_name, "Echidna");
    score_table[7].player_score = 80;
    strcpy(score_table[8].player_name, "Emu");
    score_table[8].player_score = 70;
    strcpy(score_table[9].player_name, "Possum");
    score_table[9].player_score = 60;
    strcpy(score_table[10].player_name, "Monkey");
    score_table[10].player_score = 50;
}

bool load_scores()
{
    FILE *hsf;

    hsf = fopen("highscores.bin", "rb");

    if(hsf != NULL)
    {
        if(fread(score_table, sizeof(Score), 10, hsf) != 10)
        {
            set_default_scores();
            return false;
        }
        fclose(hsf);
    }
    else
    {
        set_default_scores();
        return false;
    }

    return true;
}

bool save_scores()
{
    FILE *hsf;

    hsf = fopen("highscores.bin", "wb");

    if(hsf != NULL)
    {
        fwrite(score_table, sizeof(Score), 10, hsf);
        fclose(hsf);
        return true;
    }
    return false;
}
