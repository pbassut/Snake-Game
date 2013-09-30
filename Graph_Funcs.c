/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "main.h"

void snake_crashed(int points)
{
    BITMAP *crashed_img = (BITMAP*)img_datafile[CRASHED].dat;
    FONT *fonte         = (FONT*)fnt_datafile[COMICSANS].dat;
    SAMPLE *crashed_wav = (SAMPLE*)snd_datafile[CRASH].dat;

    fadein(crashed_img, 50, 256);
    #ifndef DEBUG
    if(game.cur_cfg.sound)
        play_sample(crashed_wav, game.cur_cfg.volume, 0, 2000, 0);
    #endif

    textprintf_centre(screen, fonte, SCREEN_W/2, SCREEN_H/2+250, WHITE, "Points: %d", points);
    rest(2000);


    if(check_hscore(points))
    {
        fadeout(50, 150);
        set_hscore(points);
    }

    fadeout(50, 256);

    return;
}

void game_introduction()
{
    BITMAP *PBLogo = (BITMAP*)img_datafile[PBASSUTLOGO].dat;


    fadein(PBLogo, 50, 256);

    register int i = 0;
    for(i = 0; (i < 2000) && (!key[KEY_ESC]); i++)
    {
        rest(1);
        if(i == 1999)
            fadeout(50, 256);
    }

    return;
}

void read_string(char *str_to, int x, int y, int size)
{
    FONT *font = (FONT*)fnt_datafile[COMICSANS].dat;
    BITMAP *buffer = create_bitmap((SCREEN_W/1.5 - SCREEN_W/3.3), (SCREEN_H/2.14 - SCREEN_H/2.4));
    int cur_pos = 0; /// Cursor position
    int the_key = 0;
    int i;
    size+=1;
    for (i = 0; i < size; i++)
            str_to[i] = '\0'; /// 'clean' the string


    while (the_key>>8 != KEY_ENTER)
    {
            the_key = readkey();

            if ((the_key & 0xff) >= ' ') /// get only valid chars
            {
                str_to[cur_pos] = the_key & 0xff;
                cur_pos++;
                if (cur_pos > size-2) cur_pos = size-2;

            }

            if (the_key >> 8 == KEY_BACKSPACE)
            {
                cur_pos--;
                str_to[cur_pos] = '\0'; /// chop the string
                if (cur_pos < 0) cur_pos = 0;
            }

            clear(buffer);
            textout(buffer, font, str_to, 0, 0, WHITE);
            draw_sprite(screen, buffer, SCREEN_W/3.125, SCREEN_H/2.12);
    }
}


void fadein(BITMAP *bmp_orig, int speed, int until)
{
    #ifndef DEBUG
    BITMAP *bmp_buff;

    if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        if (speed <= 0) speed = 16;

        int a;
        for (a = 0; a < until; a+=speed)
        {
            clear(bmp_buff);
            set_trans_blender(0,0,0,a);
            draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
            vsync();
            blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
            rest(50);
        }
        destroy_bitmap(bmp_buff);
    }
    if(until == 256)
    blit(bmp_orig, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
    #endif
}

void fadeout(int speed, int until)
{
    #ifndef DEBUG
    BITMAP *bmp_orig, *bmp_buff;

    if ((bmp_orig = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
        {
            blit(screen, bmp_orig, 0,0, 0,0, SCREEN_W, SCREEN_H);
            if (speed <= 0) speed = 16;

            for (until -= speed; until > 0; until-=speed)
            {
                clear(bmp_buff);
                set_trans_blender(0,0,0,until);
                draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
                vsync();
                blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
                rest(50);
            }
            destroy_bitmap(bmp_buff);
        }
        destroy_bitmap(bmp_orig);
    }
    #endif
}
