/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "main.h"

int main_menu()
{
    BITMAP *buffer   = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *main     = (BITMAP*)img_datafile[MAIN_MENU].dat;
    BITMAP *arrow    = (BITMAP*)img_datafile[ARROW].dat;
    MIDI *bg_music   = (MIDI*)snd_datafile[BG_MUSIC].dat;
    SAMPLE *click      = (SAMPLE*)snd_datafile[CLICK].dat;

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
            {
                play_sample(click, 255, 0, 2000, 0);
                choice--;
            }
            if((key[KEY_DOWN]) && (choice < 3))
            {
                play_sample(click, 255, 0, 2000, 0);
                choice++;
            }

            switch(choice)
            {
                case 0:
                    draw_sprite(buffer, arrow, 232, 264);
                    break;
                case 1:
                    draw_sprite(buffer, arrow, 228, 339);
                    break;
                case 2:
                    draw_sprite(buffer, arrow, 278, 411);
                    break;
                case 3:
                    draw_sprite(buffer, arrow, 325, 485);
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
    BITMAP *onLogo      = (BITMAP*)img_datafile[BUTTON_ON].dat;
    BITMAP *offLogo      = (BITMAP*)img_datafile[BUTTON_OFF].dat;
    FONT *fonte         = (FONT*)fnt_datafile[COMICSANS].dat;
    SAMPLE *click      = (SAMPLE*)snd_datafile[CLICK].dat;


    tConfig new_configuration = game.cur_cfg;

    int choice = 0;
    int arrow_pos_y[7] = {213, 254, 296, 337, 380, 508};
    int fixed_x = 125;
    fadein(main, 50, 256);

    draw_sprite(buffer, arrow, fixed_x, arrow_pos_y[0]);

    bool loop = true;

    while(loop)
    {
        clear_bitmap(buffer);
        draw_sprite(buffer, main, 0, 0);

        char *msg = (char*)malloc(sizeof(char)*10);
        char *aux = (char*)malloc(sizeof(char)*10);;
        itoa(new_configuration.resolutionx, msg, 10); strcat(msg, "x");
        itoa(new_configuration.resolutiony, aux, 10);strcat(msg, aux);
        textout_ex(buffer, fonte, msg, 680, 213, BLUE, PINK);

        if(new_configuration.window_mode) draw_sprite(buffer, onLogo, 680, 254);
        else draw_sprite(buffer, offLogo, 680, 254);

        if(new_configuration.music) draw_sprite(buffer, onLogo, 680, 296);
        else draw_sprite(buffer, offLogo, 680, 296);

        if(new_configuration.sound) draw_sprite(buffer, onLogo, 680, 337);
        else draw_sprite(buffer, offLogo, 680, 337);

        BITMAP *volume = create_bitmap(35, 30); clear_to_color(volume, PINK);
        itoa(new_configuration.volume/2.55, msg, 10);
        textout_ex(volume, fonte, msg, 0, 0, BLUE, PINK);
        draw_sprite(buffer, volume, 680, 380);

        if(key[KEY_UP])
        {
            play_sample(click, 255, 0, 2000, 0);
            choice--;
            if(choice < 0)
                choice = 5;
            draw_sprite(buffer, arrow, fixed_x, arrow_pos_y[choice]);
        }

        else if(key[KEY_DOWN])
        {
            play_sample(click, 255, 0, 2000, 0);
            choice++;
            if(choice > 5)
                choice = 0;
            draw_sprite(buffer, arrow, fixed_x, arrow_pos_y[choice]);
        }

        else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_ENTER])
        {
            play_sample(click, 255, 0, 2000, 0);
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

        draw_sprite(buffer, arrow, fixed_x, arrow_pos_y[choice]);
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
