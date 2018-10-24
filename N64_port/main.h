/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

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
	char mode;
} titlescreen;

void main_run();

void title_logic();
void story_logic();
void fleche_logic(unsigned short x, unsigned short y);
void gameplay();
void game_logic();
void gameover_logic();
short rand_a_b(short a, short b);
void reset_settings();

void possum_state();
void reset_possum_state(unsigned char mode);
void mode_gameplay(unsigned char mode);

void Set_Mode(unsigned char mode);
void Set_Titlescreen_state(unsigned char mode);

void show_warn_sign();

void Load_score();
void Save_score();

sprite_t* sprites[14];
void Load_Font();

void Print_text(int x, int y, const char *text_ex);
void Load_Picture(const char* path, unsigned char a);
