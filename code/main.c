#include "main.h"

int main()
{
	if(!init_allegro())
        exit(0);

    init_settings();
    #ifndef DEBUG
	game_introduction();
	#endif
    main_menu();

    unload_datafile(img_datafile);
    save_scores();

    return 0;
}
END_OF_MAIN();

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

void game_play()
{
    init_game();
    init_settings();

    fadeout(50, 256);

    BITMAP *snake_head[4]   = {
                                (BITMAP*)img_datafile[HEAD].dat/*load_bitmap("Data/Imagens/test.bmp", NULL)*/,
                                create_bitmap(32, 32),
                                create_bitmap(32, 32),
                                create_bitmap(32, 32)
                                };
                                clear_bitmap(snake_head[1]);
                                clear_bitmap(snake_head[2]);
                                clear_bitmap(snake_head[3]);

    BITMAP *tail            = (BITMAP*)img_datafile[TAIL].dat;
    BITMAP *food_image      = (BITMAP*)img_datafile[NORMAL_FOOD].dat;
    BITMAP *background      = (BITMAP*)img_datafile[BACKGROUND].dat;
    BITMAP *buffer          = create_bitmap(800, 600);
    register int i = 0;

    for(i = 1; i < 4; i++)
        rotate_sprite(snake_head[i], snake_head[0], 0, 0, itofix(64*i));

    fadein(background, 50, 256);

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

        rest(player.speed);
    }
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

inline BITMAP *new_food()
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

void snake_crashed(int points)
{
    BITMAP *crashed_img = (BITMAP*)img_datafile[CRASHED_IMG].dat;
    FONT *fonte         = (FONT*)fnt_datafile[COMICSANS].dat;
    SAMPLE *crashed_wav = (SAMPLE*)snd_datafile[CRASHED_WAV].dat;

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

    fadein(high_score, 10, 50);

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

            textout_ex(screen, fonte, score_table[i-1].player_name, (SCREEN_W/4.04), (SCREEN_H/12.24)*i, RED, -1);
            textout_ex(screen, fonte, score, (SCREEN_W/1.83), (SCREEN_H/12.1)*i, RED, -1);
        }
        else{
            textout_ex(screen, fonte, score_table[i-1].player_name, (SCREEN_W/4.04), (SCREEN_H/12.24)*i, WHITE, -1);
            textout_ex(screen, fonte, score, (SCREEN_W/1.83), (SCREEN_H/12.1)*i, WHITE, -1);
        }

    }
    textout_ex(screen, font, "Press any key to exit", SCREEN_W/2.5, SCREEN_H-50, WHITE, -1);

    while(!keypressed());
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

int qsort_helper_by_score(const void *e1, const void *e2)
{
    return ((const Score *)e2)->player_score - ((const Score *)e1)->player_score;
}

void sort_scores()
{
    qsort(score_table, 11, sizeof(Score), qsort_helper_by_score);
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

inline void close_game()
{
    save_scores();
    allegro_exit();
    exit(EXIT_SUCCESS);
}
