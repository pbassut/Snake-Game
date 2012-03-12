/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "Menus.h"

int main_menu()
{
    BITMAP *buffer   = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *main     = (BITMAP*)img_datafile[MAIN_MENU].dat;
    BITMAP *arrow    = (BITMAP*)img_datafile[ARROW].dat;
    MIDI *bg_music   = (MIDI*)snd_datafile[BG_MUSIC].dat;

    while(1)
    {
        #ifndef DEBUG
        if(game.cur_cfg.music)
        {
            if(!game.is_playing)
            {
                play_midi(bg_music, 1);
                game.is_playing = true;
            }
        }
        else
        {
            stop_midi();
            game.is_playing = false;
        }
        #endif
        int choice = 0;

        rest(300);

        fadein(main, 50, 256);

        while(!key[KEY_ENTER] && !key[KEY_ENTER_PAD])
        {
            blit(main, buffer, 0, 0, 0, 0, MAX_X, MAX_Y);

            if(((key[KEY_UP]) && (choice > 0)))
                choice--;
            if((key[KEY_DOWN]) && (choice < 3))
                choice++;

            switch(choice)
            {
                case 0:
                    draw_sprite(buffer, arrow, 165, 300);
                    break;
                case 1:
                    draw_sprite(buffer, arrow, 190, 355);
                    break;
                case 2:
                    draw_sprite(buffer, arrow, 200, 420);
                    break;
                case 3:
                    draw_sprite(buffer, arrow, 240, 490);
                    break;

            }
            draw_sprite(screen, buffer, 0, 0);
            readkey();
        }

        fadeout(50, 256);

        switch(choice)
        {
            case 0:
                stop_midi();
                game.is_playing = false;
                game_play();
            break;
            case 1:
                show_hscore_list(NULL);
            break;
            case 2:
                option_menu();
            break;
            case 3:
                close_game();
        }
    }
}

void option_menu(void)
{
    BITMAP *main        = (BITMAP*)img_datafile[OPTION_MENU].dat;
    BITMAP *arrow       = (BITMAP*)img_datafile[ARROW].dat;
    BITMAP *buffer      = create_bitmap (SCREEN_W, SCREEN_H);
    FONT *fonte         = (FONT*)fnt_datafile[COMICSANS].dat;

    tConfig new_configuration = game.cur_cfg;

    int choice = 0;
    int arrow_pos_y[7] = {200, 235, 275, 315, 350, 530};

    fadein(main, 50, 256);

    draw_sprite(buffer, arrow, 10, arrow_pos_y[0]);

    bool loop = true;

    while(loop)
    {
        draw_sprite(buffer, main, 0, 0);

        textprintf_centre(buffer, fonte, 700, 220, BLUE, "%dx%d", new_configuration.resolutionx, new_configuration.resolutiony);
        textprintf_centre(buffer, fonte, 680, 250, BLUE, "%s", new_configuration.window_mode?"ON":"OFF");
        textprintf_centre(buffer, fonte, 680, 285, BLUE, "%s", new_configuration.music?"ON":"OFF");
        textprintf_centre(buffer, fonte, 680, 320, BLUE, "%s", new_configuration.sound?"ON":"OFF");
        textprintf_centre(buffer, fonte, 680, 360, BLUE, "%.0f", (new_configuration.volume)/2.55);


        if(key[KEY_UP])
        {
            choice--;
            if(choice < 0)
                choice = 5;
            draw_sprite(buffer, arrow, 10, arrow_pos_y[choice]);
        }

        else if(key[KEY_DOWN])
        {
            choice++;
            if(choice > 5)
                choice = 0;
            draw_sprite(buffer, arrow, 10, arrow_pos_y[choice]);
        }

        else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_ENTER])
        {
            switch(choice)
            {
                case 0:
                break;
                case 1:
                    if(new_configuration.window_mode)
                        new_configuration.window_mode = false;
                    else new_configuration.window_mode = true;
                break;
                case 2:
                    if(new_configuration.music)
                        new_configuration.music = false;
                    else new_configuration.music = true;
                break;
                case 3:
                    if(new_configuration.sound)
                        new_configuration.sound = false;
                    else new_configuration.sound = true;
                break;
                case 4:
                    if(key[KEY_RIGHT] && (new_configuration.volume < 255))
                        new_configuration.volume = (new_configuration.volume > 255)?255:((int)new_configuration.volume+2.55);
                    else if(key[KEY_LEFT] && (new_configuration.volume > 0))
                        new_configuration.volume = (new_configuration.volume < 0)?0:((int)new_configuration.volume-2.55);
                break;
                case 5:
                    loop = false;
                break;

            }
        }
        else if(key[KEY_ESC])
            break;

        draw_sprite(buffer, arrow, 10, arrow_pos_y[choice]);
        draw_sprite(screen, buffer, 0, 0);

        rest(100);
    }

    apply_settings(new_configuration);

    fadeout(50, 256);

    return;
}

void apply_settings(tConfig new_config)
{
    int resolutionx = new_config.resolutionx;
    int resolutiony = new_config.resolutiony;
    int window_mode = new_config.window_mode;
    int volume      = new_config.volume;

    if(resolutionx != game.cur_cfg.resolutionx)
        if(set_gfx_mode(window_mode, resolutionx, resolutiony, V_MAX_X, V_MAX_Y) < 0)
            exit(0);

    set_volume(volume, volume);

    game.cur_cfg = new_config;

    return;
}
