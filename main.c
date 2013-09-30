/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File: main.c
**/

#include "main.h"

tSnake player;
tGame game;
DATAFILE *img_datafile;
DATAFILE *snd_datafile;
DATAFILE *fnt_datafile;
Score score_table[11];

int main()
{
    img_datafile = NULL;
    snd_datafile = NULL;
    fnt_datafile = NULL;
	if(!init_allegro())
        exit(0);

    init_settings();
	game_introduction();
    main_menu();
    init_game();

    return 0;
}
END_OF_MAIN();

inline void close_game()
{
    unload_datafile(img_datafile);
    save_scores();
    allegro_exit();
    exit(EXIT_SUCCESS);
}
