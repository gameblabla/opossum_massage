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
#include <math.h>
#include <time.h>

#include "main.h"


int main ( int argc, char* argv[] )
{
    Init_video();
    Load_Image(1,"data/title.bmp");
    Load_Image(2,"data/red.bmp");
    Load_Image(3,"data/fleche.bmp");
   
    Load_Image(4,"data/story1.bmp");
    Load_Image(5,"data/story2.bmp");
    Load_Image(6,"data/story3.bmp");
    
    Load_Image(7,"data/game.bmp");
    Load_Image(8,"data/score.bmp");
    Load_Image(9,"data/white.bmp");
    
    Load_Image(10,"data/ready.bmp");
    Load_Image(11,"data/oldman.bmp");
    Load_Image(12,"data/go.bmp");
    Load_Image(13,"data/gameover.bmp");
    Load_Image(14,"data/hi.bmp");
    Load_Image(15,"data/warn.bmp");
    
    Load_Image(16,"data/possum.bmp");
    
    Load_Font();

	highscore = 0;
	score = 0;
    reset_settings();
    Load_score();

    while (!done)
    {
        Clear_screen();
        Controls();
        
		if (BUTTON.QUIT) done = 1;
        
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

        Update_video();

    } 
    
    // end main loop

    Clearing();
    return 0;
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
	
	Put_image(7, 0, 0);
	Put_image(8, 100, 7);
	Put_image(9, 104+carre_tension, 22);
    sprintf(string_score, "%d", score);
	Print_text(160, 7, string_score, 0);
	
	Put_sprite(11, 104, 108, 96, 100, animation_oldman_frames[animation_oldman]);
	
	switch (start_game)
	{
		case 0:
			Put_image(10, 136, 112);
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
				game_mode = GAMEOVER;
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
			
			if (BUTTON.A)
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

void title_logic()
{
	static unsigned char red_time = 0;
	static unsigned char press_start_time = 0;
	Put_image(1, 0, 0);
	Put_image(16, 112, 112);
	
	Put_image(14, 240, 208);
	Print_text(240, 224, highscore_string, 1);	
	
	switch(titlescreen.state)
	{
		case 0:
			press_start_time++;
			
			if (press_start_time > 60)
				press_start_time = 0;
			else if (press_start_time < 30)
				Print_text(104, 172, "Space  to  continue", 0);
			
			if (BUTTON.START)
			{
				titlescreen.state = 1;
			}
			
		break;
		case 1:
			Print_text(136, 148, "Mood mode", 0);
			Print_text(136, 164, "Linear mode", 0);
			fleche_logic(112,146 + (titlescreen.mode * 14));
			
			if (BUTTON.UP) 
				titlescreen.mode = 0;
			else if (BUTTON.DOWN) 
				titlescreen.mode = 1;
			
			if (BUTTON.A)
			{
				titlescreen.state = 2;
				red_time = 0;
				start_time = 0;
			}
			else if (BUTTON.B)
			{
				titlescreen.state = 0;
			}
		break;
		case 2:
			start_time++;
			red_time++;
				
			if (start_time < 20)
			{
				if (red_time < 4)
				{
					Put_image(2, 0, 0);
				}
				else if (red_time > 8)
				{
					Put_image(1, 0, 0);
				}
				else
				{
					red_time = 0;
				}
			}
			else if (start_time > 80)
			{
				game_mode = STORY;
			}
		break;
	}
}


void story_logic()
{
	if (image_story < 3) Put_image(image_story+4, story_x, 0);
	
	if (BUTTON.A && story_transition == 0)
	{
		story_transition = 1;
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
			if (image_story == 3) game_mode = GAME;
		}
	}
	else
	{
		fleche_logic(304,224);
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
	
	Put_sprite(3, x, y, 16, 16, fleche_animation[fleche_frame]);
}

void gameover_logic()
{
	short possum_x_positions[5] = {-20, 0, 20, 126};
	static unsigned char animation_possum = 0;
	
	animation_possum++;
	gameover_time++;
	if (animation_possum > 4) animation_possum = 0;
	
	Put_image(12, possum_x_positions[animation_possum], 0);
	Put_image(13, 80, 16);

	if (gameover_time > 120)
	{
		reset_settings();
	}
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
	Put_sprite(15, 64, 14, 16, 16, warn_sign_frame[i]);
}

void possum_state()
{
	switch(possum.state)
	{
		case 0:
			Print_text(12, 32, "Sleepy", 1);
		break;
		case 1:
			Print_text(12, 32, "Grumpy", 0);
		break;
		case 2:
			Print_text(12, 32, "Scared", 0);
		break;
		case 3:
			Print_text(12, 32, "Angry", 2);
		break;
		case 4:
			Print_text(12, 32, "SATANIC MAD", 2);
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
		sprintf(highscore_string, "%d", highscore);
		Save_score();
	}
	game_mode = TITLESCREEN;
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
}

// Used for random coordinate for the clouds
short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}

void mode_gameplay(unsigned char mode)
{
	switch(mode)
	{
		case 0:
			Print_text(12, 16, "MOOD :", 0);
			possum.time++;
			
			possum_state();
			
			if (possum.time > possum.time_needed)
				reset_possum_state(1);
				
			if (possum.time+45 > possum.time_needed)
				show_warn_sign();
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

void Load_score()
{
	FILE* file;
	file = fopen("./possum.save", "r+");
		
	if (file)
	{
		 highscore = ReadLongLittleEndian (file);
	}
	else
	{
		// If the file does not exist then create it
		file = fopen("./possum.save", "w");
		highscore = 0;
		WriteIntLittleEndian((uint32_t)highscore, file);
	}
	
	sprintf(highscore_string, "%d", highscore);
		
	fclose(file);
}

void Save_score()
{
	FILE* file;
	file = fopen("./possum.save", "w+");
	
	WriteIntLittleEndian((uint32_t)highscore, file);
		
	fclose(file);
}


uint32_t ReadLongLittleEndian (FILE* output) 
{
    uint32_t temp_long;
    temp_long  = fgetc(output);
    temp_long |= fgetc(output) << 8;
    temp_long |= fgetc(output) << 16;
    temp_long |= fgetc(output) << 24;
    return temp_long;
}

void WriteIntLittleEndian (uint32_t long_in, FILE* output)  
{
    fputc (((int)(long_in      )) & 0xFF, output);
    fputc (((int)(long_in >>  8)) & 0xFF, output);
    fputc (((int)(long_in >> 16)) & 0xFF, output);
    fputc (((int)(long_in >> 24)) & 0xFF, output);
    fflush(output);
}
