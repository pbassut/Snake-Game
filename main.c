/**
Snake-Game:
A remake of the mobile game from nokia(don't know what model)
Made by Patrick Farias Bassut Souza <patrickbassut@hotmail.com

File: main.c
**/

#include "main.h"

int main()
{
	if(!init_allegro())
        exit(0);

    init_settings();
	game_introduction();
    main_menu();

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
