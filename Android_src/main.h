/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include "API.h"
#include "INPUT.h"

#define TITLESCREEN 0
#define STORY 1
#define GAME 2
#define GAMEOVER 3

char string_score[12];
char highscore_string[12];
unsigned int highscore = 0;
unsigned int score = 0;

unsigned char image_story = 0;
unsigned char story_transition = 0;
short story_x = 0;
unsigned char start_game = 0;
unsigned short gameover_time = 0;
unsigned char carre_tension = 52;

unsigned char game_mode;
unsigned char start_pressed;
unsigned char start_time;

char story_text[3][6][48] =
{
	{
		{"2017 AD"},
		{"TRUMP IS NOW PRESIDENT OF THE USA."},
		{"AS PROMISED, HE WILL BAN ALIENS FROM"},
		{"MURICA."},
		{""},
		{""}
	},
	{
		{"ALIENS FELT OFFENDED BY THAT."},
		{"AFTER MUCH DEBATE,"},
		{"THE ALIEN ASSEMBLY DECIDED TO INVADE"},
		{"EARTH AND KILL ALL HUMANS ON IT."},
		{"EARTH IS NOW IN DANGER."},
		{"OPOSSUMS ARE OUR ONLY HOPE."}
	},
	{
		{"HOWEVER, OPOSSUMS NEED TO BE MASSAGED"},
		{"TO REPEL THE ALIENS BACK."},
		{"PLEASE CHIN,"},
		{"SAVE OUR GOD, PROTECT MURICA..."},
		{"FOR GREAT JUSTICE !!!"},
		{"GOOD LUCK !"}
	}
};

struct opossum_
{
	unsigned char state;
	unsigned char time;
	unsigned char time_needed;
} possum;

struct title
{
	unsigned char state;
	unsigned char mode;
} titlescreen;

void main_run();

void title_logic();
void story_logic();
void fleche_logic(unsigned short x, unsigned short y);
void gameplay();
void game_logic();
void gameover_logic();

void Load_Font();
void Print_text(int x, int y, char *text_ex, unsigned char color);

short rand_a_b(short a, short b);
void reset_settings();

void possum_state();
void reset_possum_state(unsigned char mode);
void mode_gameplay(unsigned char mode);

void Set_Mode(unsigned char mode);
void Set_Titlescreen_state(unsigned char mode);

void show_warn_sign();

#define uint32_t unsigned int

#ifdef SAVE
uint32_t ReadLongLittleEndian (FILE* output) ;
void WriteIntLittleEndian (uint32_t long_in, FILE* output);
#endif
void Load_score();
void Save_score();


/* Print text on-screen, need Load_font before using this */
void Print_text(int x, int y, char *text_ex, unsigned char color)
{
	int i = 0;
	for (i=0;text_ex[i]!='\0';i++)
	{
		Put_sprite(100, x + (8 * i), y, 8, 8, text_ex[i]-33, 1);
	}
}

void Load_Font()
{
	Load_Image(100,"data/font.bmp", 1);
}
