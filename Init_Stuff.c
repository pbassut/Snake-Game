/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File:
Description:
**/

#include "main.h"


bool init_allegro()
{
	allegro_init();

    install_timer();
	install_keyboard();
	install_mouse();

	srand(time(NULL));
	if(install_sound(DIGI_CARD,MIDI_CARD,NULL) < 0)
        exit(0);

	set_color_depth(COLOR_BITS);

	if (set_gfx_mode(VIDEO_CARD, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
		exit(0);

	set_palette(desktop_palette);

	set_window_title(WINDOW_TITLE);
	set_close_button_callback(close_game);

    img_datafile = load_datafile("Data/Images/Images.dat");
    snd_datafile = load_datafile("Data/Sounds/Sounds.dat");
    fnt_datafile = load_datafile("Data/Fonts/Fonts.dat");

    if(!img_datafile || !snd_datafile || !fnt_datafile)
    {
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       allegro_message("Could not load datafile or the datafile wasnt found!");
       exit(EXIT_FAILURE);
    }

	return true;
}

bool init_game()
{
    /** Initializes player stuff **/
    player.pos.x = (MAX_X / 2) - 19;
    player.pos.y = (MAX_Y / 2) - 19;

    register int i;

    for(i = 0; i < 400; i++)
    {
        player.prev_snake_pos_x[i] = 2000;
        player.prev_snake_pos_y[i] = 2000;
    }

    player.tails_number = START_TAILS;
    player.direction = WEST;
    player.speed = INIT_SPEED;

    return true;
    /** The end of player stuff **/
}

bool init_settings()
{
     /** Initializes game stuff **/
    register int i = 0;
    static bool already_loaded = false;

    if(!already_loaded)
    {
        game.cur_cfg.window_mode = VIDEO_CARD;
        game.cur_cfg.resolutionx = MAX_X;
        game.cur_cfg.resolutiony = MAX_Y;
        game.cur_cfg.changed = false;
        game.cur_cfg.music = true;
        game.cur_cfg.sound = true;
        game.cur_cfg.volume = 255;
        game.is_playing = false;

        load_scores();
        already_loaded = true;
    }

    game.possible_food[0].x = 18;
    game.possible_food[0].y = 17;

    for(i = 1; i < 24; i++)
    {
        game.possible_food[i].x = game.possible_food[i - 1].x + 33; //744,711,678,645,612,579,546,513,480,447,414,381,348,315,282,249,216,183,150,117,84,51,18}; // Determina os possiveis lugares X aonde poderá 'nascer' a food.
        if(i < 18) game.possible_food[i].y = game.possible_food[i - 1].y + 33; // {17,50,83,116,149,182,215,248,281,314,347,380,413,446,479,512,545}; // Determinar os possiveis lugares Y aonde poderá 'nascer a food.
        else game.possible_food[i].y = 0;
    }

    game.food.x = game.possible_food[rand() % 23].x;
    game.food.y = game.possible_food[rand() % 17].y;
    game.points = 0;
    game.speed_food_time = clock();
    game.is_paused = false;
    game.food_type = NORMAL_FOOD;

    /** The end of game stuff **/
    return true;
}
