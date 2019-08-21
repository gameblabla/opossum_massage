/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <libdragon.h>
#include "audio.h"

#include "main.h"

static struct controller_data keys_down;
static struct controller_data keys;

static sprite_t* sprites[14];

static gamma_t gamma_select;
static antialias_t aa_select;
static int gamma_option = 0, aa_option = 0, selection_option = 0;
static display_context_t disp = 0;

static audio_t *g_audio = NULL;

int main(void)
{	
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    dfs_init( DFS_DEFAULT_LOCATION );
    //rdp_init();
    controller_init();
    //timer_init();
    
	Load_Picture("/fleche.sprite",1);
	Load_Picture("/oldman.sprite",7);
	Load_Picture("/go.sprite",8);
	Load_Picture("/gameover.sprite",9);
	Load_Picture("/warn.sprite",10);
	Load_Picture("/possum.sprite",11);
	Load_Picture("/white.sprite",13);
	
    g_audio = audio_setup( FREQUENCY_11KHZ, 4);
    audio_write_silence();
    
    audio_load_pcm(g_audio, 0, "/confirm.raw");
    audio_load_pcm(g_audio, 1, "/start.raw");
    audio_load_pcm(g_audio, 2, "/cancel.raw");
    audio_load_pcm(g_audio, 3, "/warn.raw");
    
    Load_Font();
    reset_settings();
    Load_score();

	highscore = 0;
	score = 0;

    while (1)
    {
		disp = 0;

        /* Grab a render buffer */
        while( !(disp = display_lock()) )
        {}
        
		main_run();
		
		display_show(disp);
		
		controller_scan();
        keys_down = get_keys_down();
		keys = get_keys_held();
		
        audio_tick( g_audio );
    } 
    
    // end main loop

    return 0;
}



void main_run()
{

	switch(game_mode)
	{
		case TITLESCREEN:
			title_logic();
		break;
		case STORY:
			story_logic();
		break;
		case GAME:
			gameplay();
		break;
		case GAMEOVER:
			gameover_logic();
		break;
	}
}

void title_logic()
{
	static uint8_t red_time = 0;
	static uint8_t press_start_time = 0;
	
	graphics_draw_sprite(disp, 0, 0, sprites[0]);
	graphics_draw_sprite_trans(disp, 112, 112, sprites[11]);
	
	Print_text(10, 212, "COPYWRONG 2016");
	Print_text(10, 224, "DJ OMNIMAGA, GAMEBLABLA");
	
	Print_text(240, 212, "HISCORE");
	Print_text(240, 224, highscore_string);	
	
	switch(titlescreen.state)
	{
		case 0:
			press_start_time++;
			
			if (press_start_time > 60)
				press_start_time = 0;
			else if (press_start_time < 30)
				Print_text(94, 172, "START TO CONTINUE");
			
			if (keys.c[0].start || keys.c[0].A) 
			{
				red_time = 0;
				Set_Titlescreen_state(1);
			}
			
		break;
		case 1:
			Print_text(136, 148, "MOOD MODE");
			Print_text(136, 164, "LINEAR MODE");
			//Print_text(136, 180, "OPTIONS");
			
			fleche_logic(112,146 + (titlescreen.mode * 14));
			
			if (keys_down.c[0].up || (keys_down.c[0].y > 64)) 
			{
				if (titlescreen.mode > 0) titlescreen.mode--;
				if (titlescreen.mode < 0) titlescreen.mode = 0;
			}
			else if (keys_down.c[0].down || (keys_down.c[0].y < -64)) 
			{
				titlescreen.mode++;
				if (titlescreen.mode > 1) titlescreen.mode = 1;
			}
			
			if (keys_down.c[0].start || keys_down.c[0].A)
			{
				if (titlescreen.mode == 2) 
				Set_Titlescreen_state(3);
				else 
				Set_Titlescreen_state(2);
			}
			else if (keys_down.c[0].B)
			{
				Set_Titlescreen_state(0);
			}
		break;
		case 2:
			start_time++;
			red_time++;
				
			if (start_time < 20)
			{
				if (red_time < 4)
				{
					graphics_draw_box(disp,0,0,320,240,graphics_make_color(255,0,0,255));
				}
				else if (red_time > 8)
				{
					graphics_draw_sprite(disp, 0, 0, sprites[0]);
				}
				else
				{
					red_time = 0;
				}
			}
			else if (start_time > 80)
			{
				Set_Mode(STORY);
			}
		break;
		case 3:
			Print_text(80, 148, "GAMMA:");
			Print_text(80, 164, "AA   :");
			Print_text(80, 180, "APPLY MODIFICATIONS");
			fleche_logic(60,146 + (selection_option * 14));
			
			switch(gamma_option)
			{
				case 0:
					Print_text(134, 148, "NO CORRECTION");
				break;
				case 1:
					Print_text(134, 148, "CORRECTION");
				break;
				case 2:
					Print_text(134, 148, "HARDWARE DITHER");
				break;
			}
			switch(aa_option)
			{
				case 0:
					Print_text(134, 164, "NO ANTI ALIASING");
				break;
				case 1:
					Print_text(134, 164, "ANTI ALIASING");
				break;
				case 2:
					Print_text(134, 164, "AA AND RESAMPLING");
				break;
			}
			
			if (keys_down.c[0].up || (keys_down.c[0].y > 64)) 
			{
				if (selection_option > 0) selection_option--;
				if (selection_option < 0) selection_option = 0;
			}
			else if (keys_down.c[0].down || (keys_down.c[0].y < -64)) 
			{
				selection_option++;
				if (selection_option > 2) selection_option = 2;
			}
			
			if (keys_down.c[0].left || (keys_down.c[0].x < -64)) 
			{
				if (selection_option == 0)
				{
					gamma_option--;
					if (gamma_option < 0) gamma_option = 0;			
				}
				else if (selection_option == 1)
				{
					aa_option--;
					if (aa_option < 0) aa_option = 0;			
				}
			}
			else if (keys_down.c[0].right || (keys_down.c[0].x > 64)) 
			{
				if (selection_option == 0)
				{
					gamma_option++;
					if (gamma_option > 2) gamma_option = 2;			
				}
				else if (selection_option == 1)
				{
					aa_option++;
					if (aa_option > 2) aa_option = 2;			
				}
			}
			
			if (keys_down.c[0].A && selection_option == 2) 
			{
				if (gamma_option == 0) gamma_select = GAMMA_NONE;
				else if (gamma_option == 1) gamma_select = GAMMA_CORRECT;
				else if (gamma_option == 2) gamma_select = GAMMA_CORRECT_DITHER;
				
				if (aa_option == 0) aa_select = ANTIALIAS_OFF ;
				else if (aa_option == 1) aa_select = ANTIALIAS_RESAMPLE ;
				else if (aa_option == 2) aa_select = ANTIALIAS_RESAMPLE_FETCH_ALWAYS;
				
				display_close();
				display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, gamma_select, aa_select  );
				Set_Titlescreen_state(1);
			}
			
		break;
	}
}

void story_logic()
{
	unsigned char i;
	
	if (image_story < 3) 
	{
		graphics_draw_sprite(disp, 0, 0, sprites[0]);
		for (i=0;i<6;i++)
		{
			Print_text(12+story_x, 162+(i*12), story_text[image_story][i]);
		}
	}
	
	if (story_transition > 0)
	{
		story_x = story_x + 16;
		if (story_x > 320)
		{
			image_story++;
			story_transition = 2;
			story_x = -320;
		}
		
		if (story_transition == 2 && story_x == 0)
		{
			story_transition = 0;
			if (image_story == 3) Set_Mode(GAME);
		}
	}
	else
	{
		if ((keys_down.c[0].A || keys_down.c[0].start) && story_transition == 0)
		{
			if (image_story == 0) Load_Picture("/story2.sprite",0);
			else if (image_story == 1) Load_Picture("/story3.sprite",0);
			story_transition = 1;
		}
		fleche_logic(304,224);
	}
}


void gameplay()
{
	static unsigned char animation_oldman = 0;
	static unsigned char time_oldman = 0;
	unsigned char animation_oldman_frames[3] = {0, 1, 126};
	static unsigned char carre_time = 0;
	static unsigned char carre_reserve = 0;
	static unsigned char carre_time_reserve = 0;
	static unsigned char start_time = 0;
	unsigned char temp_random;
	
	graphics_draw_sprite(disp, 0, 0, sprites[0]);
	Print_text(100, 7, "SCORE");
	
	graphics_draw_sprite(disp, 104+carre_tension,22, sprites[13]);
	itoa (score, string_score, 10);
	Print_text(160, 7, string_score);
	
	graphics_draw_sprite_trans_stride(disp, 104, 108, sprites[7], animation_oldman_frames[animation_oldman]);
	
	switch (start_game)
	{
		case 0:
			Print_text(136, 112, "READY ?");
			start_time++;
			if (start_time > 60)
			{
				start_game = 1;
				start_time = 0;
				reset_possum_state(0);
			}
		break;	
		case 1:
			score++;
			carre_time++;
			carre_time_reserve++;
			time_oldman++;
			
			mode_gameplay(titlescreen.mode);
			
			if (carre_tension < 1 || carre_tension > 103)
			{
				Set_Mode(GAMEOVER);
			}
			
			if (carre_time_reserve > 0)
			{
				while (carre_reserve > 0) 
				{
					temp_random = rand_a_b(1, 3);
					carre_reserve = carre_reserve - 1;
					carre_tension = (carre_tension + temp_random + possum.state);
				}
				carre_time_reserve = 0;
			}
			
			if (carre_time > 0)
			{
				temp_random = 2;
				carre_tension = carre_tension - (temp_random + possum.state);
				carre_time = 0;
			}
			
			if (keys.c[0].A)
			{
				if (time_oldman > 2)
				{
					animation_oldman++;
					if (animation_oldman_frames[animation_oldman] == 126) animation_oldman = 0;
					time_oldman = 0;
				}
				carre_reserve = carre_reserve + 3;
			}
		break;		
	}

}


void gameover_logic()
{
	short possum_x_positions[5] = {-20, 0, 20, 126};
	static unsigned char animation_possum = 0;
	
	animation_possum++;
	if (animation_possum > 2) animation_possum = 0;
	
	graphics_draw_sprite(disp, possum_x_positions[animation_possum], 0, sprites[0]);
	graphics_draw_sprite_trans(disp, 80, 16, sprites[9]);

	if (keys.c[0].start || keys.c[0].A) 
	{
		reset_settings();
	}
}

void Set_Mode(unsigned char mode)
{
	game_mode = mode;
	
	audio_free_pcm(g_audio, 4);
	
	switch(game_mode)
	{
		case TITLESCREEN:
			Load_Picture("/title.sprite", 0);
			audio_load_pcm(g_audio, 4, "/bubbleyard.raw");
			audio_play_music( g_audio, 4, 1);
		break;
		
		case STORY:
			Load_Picture("/story1.sprite", 0);
			audio_load_pcm(g_audio, 4, "/story.raw");
			audio_play_music( g_audio, 4, 1);
		break;
		
		case GAME:
			Load_Picture("/game.sprite", 0);
			audio_load_pcm(g_audio, 4, "/unicorn.raw");
			audio_play_music( g_audio, 4, 1);
		break;
		
		case GAMEOVER:
			Load_Picture("/go.sprite", 0);
			audio_load_pcm(g_audio, 4, "/gameover.raw");
			audio_play_music( g_audio, 4, 0);
		break;
	}
}


void Set_Titlescreen_state(unsigned char mode)
{
	titlescreen.state = mode;
	switch(mode)
	{
		case 1:
			audio_play_sfx( g_audio, 0);
			titlescreen.mode = 0;
		break;
		case 2:
			audio_play_sfx( g_audio, 1);
			start_time = 0;
		break;
		case 0:
			audio_play_sfx( g_audio, 0);
		break;
	}
}

void mode_gameplay(unsigned char mode)
{
	switch(mode)
	{
		case 0:
			Print_text(10, 16, "MOOD:");
			possum.time++;
			
			possum_state();
			
			if (possum.time > possum.time_needed)
				reset_possum_state(1);
				
			if (possum.time+45 == possum.time_needed)
			{
				audio_play_sfx( g_audio, 3);
			}
			else if (possum.time+45 > possum.time_needed)
			{
				show_warn_sign();
			}
		break;
		case 1:
			if (score > 2000)
			{
				possum.state = 4;
			}
			else if (score > 1250)
			{
				possum.state = 3;
			}
			else if (score > 500)
			{
				possum.state = 1;
			}
			else
			{
				possum.state = 0;
			}
		break;
	}
}



void fleche_logic(unsigned short x, unsigned short y)
{
	static unsigned char fleche_frame = 0;
	static unsigned char fleche_time = 0;
	unsigned char fleche_animation[10] = {0, 1, 2, 3, 2, 1, 126};
	
	fleche_time++;
	
	if (fleche_time > 2)
	{
		fleche_frame++;
		fleche_time = 0;
	}
	
	if (fleche_animation[fleche_frame] == 126)
	{
		fleche_frame = 0;
	}
	graphics_draw_sprite_trans_stride  (disp, x, y, sprites[1], fleche_animation[fleche_frame] );
}


void show_warn_sign()
{
	static unsigned char i = 0;
	static unsigned char warn_sign_frame[2] = {0, 1};
	static unsigned char warn_sign_time = 0;
	
	warn_sign_time++;
	if (warn_sign_time > 2)
	{
		warn_sign_time = 0;
		i++;
		if (i>1) i = 0;
	}
	
	graphics_draw_sprite_trans_stride (disp, 224, 17, sprites[10], warn_sign_frame[i]);
}

void possum_state()
{
	switch(possum.state)
	{
		case 0:
			Print_text(10, 28, "SLEEPY");
		break;
		case 1:
			Print_text(10, 28, "GRUMPY");
		break;
		case 2:
			Print_text(10, 28, "SCARED");
		break;
		case 3:
			Print_text(10, 28, "ANGRY");
		break;
		case 4:
			Print_text(10, 28, "SATANIC MAD");
		break;
	}
}


void reset_possum_state(unsigned char mode)
{
	srand(time(NULL));
	possum.state = (mode == 1) ? rand_a_b(0,5) : 0;
	possum.time_needed = rand_a_b(360,480);
	possum.time = 0;
}

void reset_settings()
{
	if (score > highscore) 
	{
		highscore = score;
	}
	itoa (highscore, highscore_string, 10);
	Save_score();
	
	start_pressed = 0;
	start_time = 0;
	image_story = 0;
	story_transition = 0;
	story_x = 0;
	start_game = 0;
	gameover_time = 0;
	score = 0;
	carre_tension = 52;
	possum.state = 0;
	possum.time = 0;
	titlescreen.state = 0;
	titlescreen.mode = 0;
	Set_Mode(TITLESCREEN);
}


short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}



void Load_score()
{
}

void Save_score()
{
}

void Load_Picture(const char* path, unsigned char a)
{
	int fp;
	
	if (sprites[a] != NULL)
	{
		free(sprites[a]);
		sprites[a] = NULL;
	}
	
	fp = dfs_open(path);
    sprites[a] = malloc( dfs_size( fp ) );
    dfs_read( sprites[a], sizeof(uint8_t), dfs_size( fp ), fp );
    dfs_close( fp );
}


/* Print text on-screen, need Load_font before using this */
void Print_text(int x, int y, const char *text_ex)
{
	int i = 0;
	for (i=0;text_ex[i]!='\0';i++)
	{
		if (text_ex[i] != ' ' && x < 312)
		{
			graphics_draw_sprite_trans_stride  (disp, x + (8 * i), y, sprites[12], text_ex[i]-32 );
		}
	}
}

void Load_Font()
{
	int fp;
	
	if (sprites[12] != NULL)
	{
		free(sprites[12]);
		sprites[12] = NULL;
	}

	fp = dfs_open("/font.sprite");
    sprites[12] = malloc( dfs_size( fp ) );
    dfs_read( sprites[12], 1, dfs_size( fp ), fp );
    dfs_close( fp );
}
