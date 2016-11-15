/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include "WRAPPER/API.h"
#include "WRAPPER/INPUT.h"

#define TITLESCREEN 0
#define STORY 1
#define GAME 2
#define GAMEOVER 3

char string_score[12];
char highscore_string[12];
unsigned int highscore = 0;
unsigned int score = 0;

var image_story = 0;
var story_transition = 0;
short story_x = 0;
var start_game = 0;
unsigned short gameover_time = 0;
var carre_tension = 52;

var game_mode;
var start_pressed;
var start_time;

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
	var state;
	var time;
	var time_needed;
} possum;

struct title
{
	var state;
	var mode;
} titlescreen;

function main_run();

function title_logic();
function story_logic();
function fleche_logic( x, y);
function gameplay();
function game_logic();
function gameover_logic();

function Load_Font();
function Print_text(x, y, text_ex, color);

function rand_a_b(a, b);
function reset_settings();

function possum_state();
function reset_possum_state(var mode);
function mode_gameplay(var mode);

function Set_Mode(var mode);
function Set_Titlescreen_state(var mode);

function show_warn_sign();

/* Print text on-screen, need Load_font before using this */
function Print_text(int x, int y, char *text_ex, var color)
{
	var i = 0;
	for (i=0;text_ex[i]!='\0';i++)
	{
		Put_sprite(100, x + (8 * i), y, 8, 8, text_ex[i]-33);
	}
}

function Load_Font()
{
	Load_Image(100,"data/font.bmp");
}
