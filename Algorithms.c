/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "main.h"

int qsort_helper_by_score(const void *e1, const void *e2)
{
    return ((const Score *)e2)->player_score - ((const Score *)e1)->player_score;
}

void sort_scores()
{
    qsort(score_table, 11, sizeof(Score), qsort_helper_by_score);
}

bool check_collision(int collision_type)
{
    switch(collision_type)
    {
        case ITSELF_COLLISION:
        {
            register int i = 0;
            for(i = 0; i <= player.tails_number; i++)
                if((player.pos.x == player.prev_snake_pos_x[i]) && (player.pos.y == player.prev_snake_pos_y[i]))
                    return true;
        }
        break;
        case WALL_COLLISION:
        {
            if(player.pos.x < LEFT_BORDER)
                    return true;
            else if(player.pos.x > RIGHT_BORDER)
                    return true;
            else if(player.pos.y > BOTTOM_BORDER)
                    return true;
            else if(player.pos.y < TOP_BORDER)
                    return true;
        }
        break;

        case FOOD_COLLISION:
        if(game.speed_food_time <= (clock() - 5000))
            player.speed = INIT_SPEED;

        if((player.pos.x == game.food.x) && (player.pos.y == game.food.y))
        {
            SAMPLE *swallow = (SAMPLE*)snd_datafile[SWALLOW].dat;
            play_sample(swallow, game.cur_cfg.volume, 0, 2000, 0);

            player.tails_number++;

            if(game.food_type == NORMAL_FOOD)
                game.points += 10;
            else if(game.food_type == SPEED_FOOD)
            {
                player.speed = INIT_SPEED * 0.5;
                game.speed_food_time = clock();
                game.points += 20;
            }

            return true;
        }
        break;
    }

    return false;
}

BITMAP *new_food()
{
    BITMAP *food_image;
    bool loop_again;
    register int i;

    do{
        loop_again = false;
        game.food.x = game.possible_food[rand() % 23].x;
        game.food.y = game.possible_food[rand() % 17].y;

        for(i = 0; i < player.tails_number; i++)
            if((game.food.x == player.prev_snake_pos_x[i]) && (game.food.y == player.prev_snake_pos_y[i]))
                loop_again = true;

    }while(((game.food.x == player.pos.x) && (game.food.y == player.pos.y)) || loop_again);

    if((rand() % 100) >= 80)
    {
        food_image = (BITMAP*)img_datafile[SPEED_FOOD].dat;
        game.food_type = SPEED_FOOD;
    }
    else{
        food_image = (BITMAP*)img_datafile[NORMAL_FOOD].dat;
        game.food_type = NORMAL_FOOD;
    }

    return food_image;
}
