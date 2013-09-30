/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "main.h"

void game_play()
{
    init_game();
    init_settings();

    fadeout(50, 256);

    BITMAP *snake_head[4]   = {
                                (BITMAP*)img_datafile[HEAD].dat,
                                create_bitmap(32, 32),
                                create_bitmap(32, 32),
                                create_bitmap(32, 32)
                                };
                                rectfill(snake_head[1], 0, 0, snake_head[1]->w, snake_head[1]->h, PINK);
                                rectfill(snake_head[2], 0, 0, snake_head[1]->w, snake_head[1]->h, PINK);
                                rectfill(snake_head[3], 0, 0, snake_head[1]->w, snake_head[1]->h, PINK);

    BITMAP *tail            = (BITMAP*)img_datafile[TAIL].dat;
    BITMAP *food_image      = (BITMAP*)img_datafile[NORMAL_FOOD].dat;
    BITMAP *background      = (BITMAP*)img_datafile[BACKGROUND].dat;
    BITMAP *buffer          = create_bitmap(800, 600);
    register int i = 0;

    for(i = 1; i < 4; i++)
        rotate_sprite(snake_head[i], snake_head[0], 0, 0, itofix(64*i));

    fadein(background, 50, 256);
    clock_t seconds = clock();
    while (!key[KEY_ESC])
    {
        #ifdef DEBUG
        if(key[KEY_ASTERISK]) player.tails_number += 1;
        if(key[KEY_MINUS_PAD]) player.speed+=100;
        if(key[KEY_PLUS_PAD]) if(player.speed!=100) player.speed-=100;
        #endif

        if((key[KEY_UP]) && (player.direction!=SOUTH))
            player.direction = NORTH;
        if((key[KEY_DOWN]) && (player.direction!=NORTH))
            player.direction = SOUTH;
        if((key[KEY_LEFT]) && (player.direction!=EAST))
            player.direction = WEST;
        if((key[KEY_RIGHT]) && (player.direction!=WEST))
            player.direction = EAST;
        if(key[KEY_P])
        {
            fadeout(50, 256);
            textprintf_centre(screen, font, SCREEN_W/2, SCREEN_H/2, WHITE, "You have paused the game. Press 'P' again to continue.");
            while(!key[KEY_P]);
            fadein(buffer, 50, 256),
            game.is_paused = false;
        }

        draw_sprite(buffer, background, 0, 0);

        if(((double)(clock() - seconds) / CLOCKS_PER_SEC) < player.speed)
            continue;

        seconds = clock();

        player.prev_snake_pos_x[0] = player.pos.x;
        player.prev_snake_pos_y[0] = player.pos.y;

        for(i = player.tails_number; i >= 1; i--)
        {
            player.prev_snake_pos_x[i] = player.prev_snake_pos_x[i - 1];
            player.prev_snake_pos_y[i] = player.prev_snake_pos_y[i - 1];
        }

        if(check_collision(FOOD_COLLISION))
            food_image = new_food();

        for(i = 0; i <= player.tails_number; i++)
            draw_sprite(buffer, tail, player.prev_snake_pos_x[i], player.prev_snake_pos_y[i]);


        draw_sprite(buffer, food_image, game.food.x, game.food.y);

        if(player.direction == NORTH)
        {
            player.pos.y -= 33;
            draw_sprite(buffer, snake_head[NORTH], player.pos.x, player.pos.y);
        }
        else if(player.direction == SOUTH)
        {
            player.pos.y += 33;
            draw_sprite(buffer, snake_head[SOUTH], player.pos.x, player.pos.y);
        }
        else if(player.direction == EAST)
        {
            player.pos.x += 33;
            draw_sprite(buffer, snake_head[EAST], player.pos.x, player.pos.y);
        }
        else if(player.direction == WEST)
        {
            player.pos.x -= 33;
            draw_sprite(buffer, snake_head[WEST], player.pos.x, player.pos.y);
        }

        if(check_collision(WALL_COLLISION) || check_collision(ITSELF_COLLISION))
        {
            snake_crashed(game.points);
            return;
        }
        draw_sprite(screen, buffer, 0, 0);
    }
}
