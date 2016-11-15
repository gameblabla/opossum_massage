/*
	Copyright (C) 2016 Gameblabla
	Licensed under the GAMEBLABLA's VIDEO GAME PUBLIC LICENSE version 1.0.
	See COPYING file for more details.
	If a COPYING file was not provided, you can ask the copyright holder for a copy at :
	<gameblabla@openmailbox.org>
		
	This Program is distributed without WITHOUT ANY WARRANTY; 
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
	See the GAMEBLABLA's VIDEO GAME PUBLIC LICENSE for more details.
*/

/*
 *	This javascript version is  
 * 
*/

var hand_inst_anim;
var hand_inst_time;

var inst_scr_alpha;
var inst_scr_time;

var lvl_struct_level;
var lvl_struct_exp;
var lvl_struct_latency;
var lvl_struct_latency_time;

var lvl_struct_position;
var lvl_struct_color;
var lvl_struct_chain;

var right_spot = Array(5);
var left_spot = Array(5);
var up_spot = Array(5);
var down_spot = Array(5);
var score_showed = Array(7);
var highscore_show = Array(7);

var time_game = 0;
var game_status = 0;
var frame_gamemo = 0;

var lives = 3;
var score = 0;
var highscore = 0;
var game_mode = 0;
var stop_game = 0;
var start_beginning = 2;
var pts_live = 0;
var go_time = 0;
var black_transition_alpha = 0;

var img_memory = Array(20);
var precalculed_fps = 1000/60;

var background;
var first_layer;
var hand_frames = [0,1,2,3,2,1,128];
var time_press = 0;

var poss_x, poss_y;
var tapped;
var sqr_frame = 0;
var sound = Array(15);
var isoggsupported = 0;
var keyboard = 0;
var score_togive;

var touch_state = 0, touch_state_time = 0;
var hammertime;


window.onload = function() 
{
	var i;
	first_layer = document.getElementById('canvas');
	background = first_layer.getContext('2d'); // A layer for the background
	mc = new Hammer(first_layer);
	
	for (i=0;i<7;i++)
	{
		score_showed[i] = 0;
		highscore_show[i] = 0;
	}
	
	highscore = localStorage.getItem("killminds-highscore");
	Update_highscore();
	
	for (i=0;i<17;i++)
	{
		img_memory[i] = new Image();
	}
	
	if (!buzz.isSupported()) 
	{
		alert("Your browser is too old, time to update!");
	}	
	else if (!buzz.isOGGSupported()) 
	{
		alert("Your browser doesn't support OGG Format...");
	}
	
	if (buzz.isOGGSupported())
	{
		isoggsupported = 1;
	}
	
	if (isoggsupported == 1)
	{
		sound[0] = new buzz.sound("data/gameover.ogg");
		sound[1] = new buzz.sound("data/miss.ogg");
		sound[2] = new buzz.sound("data/move1.ogg");
		sound[3] = new buzz.sound("data/move2.ogg");
		sound[4] = new buzz.sound("data/move3.ogg");
		sound[5] = new buzz.sound("data/move4.ogg");
		sound[6] = new buzz.sound("data/fullsquare.ogg");
		
		sound[7] = new buzz.sound("data/upleft.ogg");
		sound[8] = new buzz.sound("data/upright.ogg");
		sound[9] = new buzz.sound("data/downleft.ogg");
		sound[10] = new buzz.sound("data/downright.ogg");
		
		for (i=0;i<11;i++)
		{
			sound[i].load();
		}
	}
	
	img_memory[0].src = 'data/title.png';
	img_memory[1].src = 'data/live_chiffre.png';
	img_memory[2].src = 'data/all.png';
	img_memory[3].src = 'data/trophees.png';
	img_memory[4].src = 'data/heart.png';
	img_memory[5].src = 'data/score.png';
	img_memory[6].src = 'data/startbutton.png';
	
	img_memory[7].src = 'data/block_left.png';
	img_memory[8].src = 'data/block_right.png';
	img_memory[9].src = 'data/block_up.png';
	img_memory[10].src = 'data/block_down.png';
	img_memory[11].src = 'data/block.png';
	img_memory[13].src = 'data/hand_anim.png';
	img_memory[14].src = 'data/inst.png';
	img_memory[15].src = 'data/copyright.png';
	img_memory[16].src = 'data/back.png';



	(function (window) 
	{
		function mainloop() 
		{
			touch();
			
			switch(game_mode)
			{
				case 0:
					background.drawImage(img_memory[0], 0, 0);
					Titlescreen();
					Show_highscore();
					Show_startbutton();
					Show_copyright();
				break;

				case 1:
					background.drawImage(img_memory[16], 0, 0);
					Show_Game();
					Show_Square();
					Reload_time();
					if (game_status == 0) Move_Square();
				break;
				
				case 2:
					background.drawImage(img_memory[16], 0, 0);
					Show_Instructions();
					Show_Square();
				break;
			}
			tapped = 0;
	    }
	    requestAnimationFrame(mainloop);
		window.setInterval(mainloop, precalculed_fps); // 60 fps	
		
	} (window));

	mc.on("tap", function(e) 
	{
		var currX, currY;
		currX = e.pointers[0].layerX;
		currY = e.pointers[0].layerY;
		poss_x = (currX*(320/$(first_layer).width())) | 0;
		poss_y = (currY*(480/$(first_layer).height())) | 0;
		tapped = 1;
	});

};  




/*
 * Controls
*/

window.addEventListener("keydown", function(evt) {
	keyboard = evt.keyCode;
}, this);

window.addEventListener("keyup", function(evt) {
	if (keyboard > 0) keyboard = 0;
}, this);

function touch()
{
	switch(touch_state)
	{
		case 0:
			if (tapped == 1)
			{
				touch_state = 1;
			}
		break;
		case 1:
			touch_state_time++;
			if (touch_state_time > 0)
			{
				touch_state = 2;
				touch_state_time = 0;
			}
		break;
		case 2:
			if (tapped == 0)
			{
				touch_state = 3;
				touch_state_time = 0;
			}
		break;
		case 3:
			touch_state_time++;
			if (touch_state_time > 1)
			{
				touch_state = 0;
				touch_state_time = 0;
			}
		break;
	}
}

function Put_sprite(a, b, c, d, e, f)
{
	background.drawImage(img_memory[a], d*f, 0, d, e, b, c, d, e);
}

function Titlescreen()
{
	if (touch_state == 1 || (keyboard == 13 || keyboard == 32))
	{
		go_ingame();
	}
}

function Show_highscore()
{
	/*for(i=0;i<7;i++)
	{
		//Put_sprite(5, 55+(i*30), 360, 30, 30, highscore_show[i], 3);
		background.drawImage(img_memory[5], 30*highscore_show[i], 0, 30, 30, 55+(i*30), 360, 30, 30);
	}*/
	background.drawImage(img_memory[5], highscore_show[0]*30, 0, 30, 30, 55, 360, 30, 30);
	background.drawImage(img_memory[5], highscore_show[1]*30, 0, 30, 30, 85, 360, 30, 30);
	background.drawImage(img_memory[5], highscore_show[2]*30, 0, 30, 30, 115, 360, 30, 30);
	background.drawImage(img_memory[5], highscore_show[3]*30, 0, 30, 30, 145, 360, 30, 30);
	background.drawImage(img_memory[5], highscore_show[4]*30, 0, 30, 30, 175, 360, 30, 30);
	background.drawImage(img_memory[5], highscore_show[5]*30, 0, 30, 30, 205, 360, 30, 30);
	background.drawImage(img_memory[5], highscore_show[6]*30, 0, 30, 30, 235, 360, 30, 30);
	
	if (highscore > 10000)
		background.drawImage(img_memory[3], 0, 0, 17, 16, 260, 400, 17, 16);
	else if (highscore > 5000)
		background.drawImage(img_memory[3], 17, 0, 17, 16, 260, 400, 17, 16);
	else if (highscore > 1000)
		background.drawImage(img_memory[3], 34, 0, 17, 16, 260, 400, 17, 16);
}

function Show_startbutton()
{
	time_press++;
	
	if (time_press < 30)
	{
		background.drawImage(img_memory[6], 60, 240);
	}
	else if (time_press > 59) 
	{
		time_press = 0;
	}
}

function Show_copyright()
{
	background.drawImage(img_memory[15], 56, 420);
}


function go_back_titlescreen()
{
	Save_highscore();
	game_mode = 0;
}

function go_ingame()
{
	RESET_EVERYTHING();
	time_game = 100;
	game_mode = 2;
}
		
function RESET_EVERYTHING()
{
	var i;
	
	time_game = 0;
	game_status = 0;
	frame_gamemo = 0;
	score = 0;
	
	lives = 3;
	
	for (i=0;i<4;i++)
	{
		right_spot[i] = 0;
		left_spot[i] = 0;
		up_spot[i] = 0;
		down_spot[i] = 0;
	}
	
	for (i=0;i<7;i++)
	{
		score_showed[i] = 0;
	}
	
	hand_inst_anim = 0;
	hand_inst_time = 0;
	
	lvl_struct_level = 0;	
	lvl_struct_exp = 0;	
	lvl_struct_latency_time = 0;
	lvl_struct_latency = 2;
	
	inst_scr_alpha = 1;
	
	if (highscore > 100)
	{
		inst_scr_time = 60*6;
	}
	else
	{
		inst_scr_time = 260;
	}
	
	square_chain = 0;
	go_time = 0;
	
	black_transition_alpha = 0;
	start_beginning = 1;
	reset_case();
}



function rand_a_b(a, b)
{
  a = Math.ceil(a);
  b = Math.floor(b);
  return Math.floor(Math.random() * (b - a + 1)) + a;
}

function calcul_score()
{
	score_showed[0] = ((score / 1000000) % 10) | 0;
	score_showed[1] = ((score / 100000) % 10) | 0;
	score_showed[2] = ((score / 10000) % 10) | 0;
	score_showed[3] = ((score / 1000) % 10) | 0;
	score_showed[4] = ((score / 100) % 10) | 0;
	score_showed[5] = ((score / 10) % 10) | 0;
	score_showed[6] = (score % 10) | 0;
}

function lose_a_life()
{
	lives = lives - 1;

	if (lives < 1)
	{
		sound[0].play();
		game_status = 2;
	}
	else
	{
		sound[1].play();
		game_status = 1;
	}
}


// Generate a random number
function reset_case() 
{
	var good, i, il;

	good = 0;

	// Avoid to give a square where it can't be placed anywhere
	// Loop until the game finds a good combinaison
	// In theory, the game could loop indefinitely but it never happened to me
	// If good = 1 then it means it found a good combinaison
	while (!good)
	{
		square_position = rand_a_b(1,12);
		
		if (up_spot[0] > 0 && right_spot[0] > 0 && left_spot[0] > 0 && down_spot[0] && (square_position == 1 || square_position == 5 || square_position == 9 || square_position == 13)) 
		{
			good = 0;
		}
		else if (up_spot[1] > 0 && right_spot[1] > 0 && left_spot[1] > 0 && down_spot[1] && (square_position == 2 || square_position == 6 || square_position == 10 || square_position == 14)) 
		{
			good = 0;
		}
		else if (up_spot[2] > 0 && right_spot[2] > 0 && left_spot[2] > 0 && down_spot[2] && (square_position == 3 || square_position == 7 || square_position == 11 || square_position == 15)) 
		{
			good = 0;
		}
		else if (up_spot[3] > 0 && right_spot[3] > 0 && left_spot[3] > 0 && down_spot[3] && (square_position == 4 || square_position == 8 || square_position == 12 || square_position == 16)) 
		{
			good = 0;
		}
		else
		{
			good = 1;
		}
	}
	
	//for(i=0;i<4;i++)
	for (i=0, il=4; i<il; i++)
	{
		if (square_position == 1+(4*i)) sound[7].play();
		if (square_position == 5+(4*i)) sound[8].play();
		if (square_position == 9+(4*i)) sound[9].play();
		if (square_position == 13+(4*i)) sound[10].play();
	}
	

	time_game = 100;
	frame_gamemo = 0;

	return 1;
}


function Move_Square()
{
	var i, il;
	var sqr_score_filled;
	var canmove_square;
	
	// Player can play after some ms (or right now if instructions are shown)
	canmove_square = (time_game < 94 && start_beginning == 0) || (time_game == 100 && start_beginning > 0);
	
	if (time_game < 1)	// Player lose a live after running out of time
	{
		lose_a_life();
		reset_case();
	}
	else if (canmove_square)
	{
		if (touch_sqr(100, 212, 97, 188) || keyboard == 38)
		{
			for (i=0, il=4; i<il; i++)
			//for(i=0;i<4;i++)
			{
				if ((square_position == 1+i || square_position == 5+i || square_position == 9+i || square_position == 13+i))
				{
					if (up_spot[i] == 0)
					{
						sound[2].play();
						if (start_beginning > 0) start_beginning--;
						up_spot[i] = square_position;
					}
					else if (up_spot[i] > 0)
					{
						lose_a_life();
					}
				}	
			}
			
			if (up_spot[0] > 0 && up_spot[1] > 0 && up_spot[2] > 0 && up_spot[3] > 0)
			{
				sqr_score_filled = check_square_score(up_spot[0],up_spot[1],up_spot[2],up_spot[3]);
				for (i=0, il=4; i<il; i++)
				//for(i=0;i<4;i++) 
				{
					up_spot[i] = 0;
				}
				score = score + sqr_score_filled;
				calcul_score();
			}

			reset_case();
		}

		else if (touch_sqr(100, 212, 292, 391) || keyboard == 40)
		{

			for (i=0, il=4; i<il; i++)
			//for(i=0;i<4;i++)
			{
				if ((square_position == 1+i || square_position == 5+i || square_position == 9+i || square_position == 13+i))
				{
					if (down_spot[i] == 0)
					{
						sound[3].play();
						if (start_beginning > 0) start_beginning--;
						down_spot[i] = square_position;
					}
					else if (down_spot[i] > 0)
					{
						lose_a_life();
					}
				}	
			}
			
			if (down_spot[0] > 0 && down_spot[1] > 0 && down_spot[2] > 0 && down_spot[3] > 0)
			{
				sqr_score_filled = check_square_score(down_spot[0],down_spot[1],down_spot[2],down_spot[3]);
				for (i=0, il=4; i<il; i++)
				//for(i=0;i<4;i++) 
				{
					down_spot[i] = 0;
				}
				score = score + sqr_score_filled;
				calcul_score();
			}

			reset_case();
		}
	
		else if (touch_sqr(4, 98, 193, 286)  || keyboard == 37)
		{
			for (i=0, il=4; i<il; i++)
			//for(i=0;i<4;i++)
			{
				if ((square_position == 1+i || square_position == 5+i || square_position == 9+i || square_position == 13+i))
				{
					if (left_spot[i] == 0)
					{
						sound[4].play();
						if (start_beginning > 0) start_beginning--;
						left_spot[i] = square_position;
					}
					else if (left_spot[i] > 0)
					{
						lose_a_life();
					}
				}	
			}

			if (left_spot[0] > 0 && left_spot[1] > 0 && left_spot[2] > 0 && left_spot[3] > 0)
			{
				sqr_score_filled = check_square_score(left_spot[0],left_spot[1],left_spot[2],left_spot[3]);
				for (i=0, il=4; i<il; i++)
				//for(i=0;i<4;i++) 
				{
					left_spot[i] = 0;
				}
				score = score + sqr_score_filled;
				calcul_score();
			}

			reset_case();
		}

		else if (touch_sqr(212, 317, 185, 286) || keyboard == 39)
		{
				for (i=0, il=4; i<il; i++)
				//for(i=0;i<4;i++)
				{
					if ((square_position == 1+i || square_position == 5+i || square_position == 9+i || square_position == 13+i))
					{
						if (right_spot[i] == 0)
						{
							sound[5].play();
							if (start_beginning > 0) start_beginning--;
							right_spot[i] = square_position;
						}
						else if (right_spot[i] > 0)
						{
							lose_a_life();
						}
					}	
				}
			
				if (right_spot[0] > 0 && right_spot[1] > 0 && right_spot[2] > 0 && right_spot[3] > 0)
				{
					sqr_score_filled = check_square_score(right_spot[0],right_spot[1],right_spot[2],right_spot[3]);
					for (i=0, il=4; i<il; i++)
					//for(i=0;i<4;i++) 
					{
						right_spot[i] = 0;
					}
					score = score + sqr_score_filled;
					calcul_score();
				}

			reset_case();
		}
	}

}

function Reload_time()
{
	lvl_struct_latency_time++;
	if (lvl_struct_latency_time > lvl_struct_latency)
	{
		if (game_status == 0) 
		{
			if (start_beginning == 0 || (start_beginning > 0 && time_game > 100))
			{
				time_game--;
			}
		}
		lvl_struct_latency_time = 0;
	}	
}

function touch_sqr(x, x2, y, y2)
{
	if (poss_x > x && poss_x < x2 && poss_y > y && poss_y < y2 && (touch_state == 1 || (keyboard == 13 || keyboard == 32)))
		return 1;
	else
		return 0;
}

function check_square_score(a, b, c, d)
{
	// Check if all squares are green
	var i, il;
	var square_tmp = Array(4);
	
	score_togive = 0;
	
	square_tmp[0] = a;
	square_tmp[1] = b;
	square_tmp[2] = c;
	square_tmp[3] = d;
	
	for (i=0, il=3; i<il; i++)
	//for(i=0;i<3;i++)
	{
		if (square_tmp[0] == 1+(4*i) && square_tmp[1] == 2+(4*i) && square_tmp[2] == 3+(4*i) && square_tmp[3] == 4+(4*i))
		{
			lvl_struct_exp = lvl_struct_exp + 50;
			pts_live += 300;
			square_chain += 1;
			score_togive = 300*square_chain;
			sound[6].play();
			return score_togive;
		}
	}
	
	square_chain = 0;
	lvl_struct_exp = lvl_struct_exp + 25;
	pts_live += 30;
	return 30;
}

function Show_Instructions()
{
	Put_slots();
	
	background.save();
	background.globalAlpha = inst_scr_alpha;
	background.drawImage(img_memory[14], 0, 0);
	background.restore();
	
	if (inst_scr_time > 0) inst_scr_time--;
	if (inst_scr_time == 0) 
	{
		if (inst_scr_alpha > 0.04) 
		{
			inst_scr_alpha = inst_scr_alpha - 0.04;
		}
		else 
		{
			inst_scr_alpha = inst_scr_alpha - inst_scr_alpha;
		}
	}
	if (inst_scr_alpha < 0.01 || touch_state == 1) 
	{
		game_mode = 1;
	}
}

function Show_Game()
{
	Put_slots();
	Put_squares();
	Put_score_lives();
	Put_time();
	Put_hands();

	if (lvl_struct_exp > 100+(lvl_struct_level*15))
	{
		lvl_struct_exp = 0;
		if (lvl_struct_level < 99) lvl_struct_level++;
		if (lvl_struct_latency > 0) lvl_struct_latency--;
	}
	
	if (pts_live > 1000)
	{
		if (lives < 9) lives++;
		pts_live = 0;
	}
}


/*
 * 
 * Display stuff 
 * 
*/


function Show_Square()
{
	background.drawImage(img_memory[2], 80*sqr_frame, 0, 80, 76, 122, 201, 80, 76);

	switch(game_status)
	{
		case 2:
			go_time++;
			
			// Show "Game Over" frame
			sqr_frame = 14;

			if (go_time > 180) go_back_titlescreen();
		break;
		
		case 1:
			sqr_frame = 13;

			if (frame_gamemo > 55)
			{
				game_status = 0;
				frame_gamemo = 0;
			}
			else 
			{
				frame_gamemo++;
			}
		break;
		
		case 0:
			// Show the square you're holding
			sqr_frame = square_position;
		break;
	}
}

function Put_slots()
{
	background.drawImage(img_memory[7], 18, 197);
	background.drawImage(img_memory[11], 118, 197);
	background.drawImage(img_memory[8], 218, 197);	
	background.drawImage(img_memory[9], 118, 97);	
	background.drawImage(img_memory[10], 118, 297);	
}

function Put_squares()
{
 	var i, il;
	//for (i=0;i<4;i++)
	for (i=0, il=4; i<il; i++)
	{
		if (left_spot[i] > 0) background.drawImage(img_memory[2], 80*left_spot[i], 0, 80, 76, 22, 201, 80, 76);
		if (right_spot[i] > 0) background.drawImage(img_memory[2], 80*right_spot[i], 0, 80, 76, 222, 201, 80, 76);
		if (up_spot[i] > 0) background.drawImage(img_memory[2], 80*up_spot[i], 0, 80, 76, 122, 101, 80, 76);
		if (down_spot[i] > 0) background.drawImage(img_memory[2], 80*down_spot[i], 0, 80, 76, 122, 301, 80, 76);
	}
}


function Put_score_lives()
{
	/*var i;
	for (i=0;i<7;i++)
	{
		background.drawImage(img_memory[5], score_showed[i]*30, 0, 30, 30, 64+(i*30), 388, 30, 30);
	}*/
	background.drawImage(img_memory[5], score_showed[0]*30, 0, 30, 30, 64, 388, 30, 30);
	background.drawImage(img_memory[5], score_showed[1]*30, 0, 30, 30, 94, 388, 30, 30);
	background.drawImage(img_memory[5], score_showed[2]*30, 0, 30, 30, 124, 388, 30, 30);
	background.drawImage(img_memory[5], score_showed[3]*30, 0, 30, 30, 154, 388, 30, 30);
	background.drawImage(img_memory[5], score_showed[4]*30, 0, 30, 30, 184, 388, 30, 30);
	background.drawImage(img_memory[5], score_showed[5]*30, 0, 30, 30, 214, 388, 30, 30);
	background.drawImage(img_memory[5], score_showed[6]*30, 0, 30, 30, 244, 388, 30, 30);
	
	//	Lives_spr
	background.drawImage(img_memory[4], 120, 424);
	background.drawImage(img_memory[1], lives*20, 0, 20, 20, 172, 432, 20, 20);
}

function Put_time()
{
	var conv_time = Array(4);
	var lowtime;
	
	if (time_game < 33) 
	{
		lowtime = 10;
	}
	else 
	{
		lowtime = 0;
	}
	
	conv_time[0] = ((time_game / 100) % 10) | 0;
	conv_time[1] = ((time_game / 10) % 10)  | 0;
	conv_time[2] = (time_game % 10) | 0;
	
	Put_sprite(5, 115, 48, 30, 30, conv_time[0]+lowtime);
	Put_sprite(5, 145, 48, 30, 30, conv_time[1]+lowtime);
	Put_sprite(5, 175, 48, 30, 30, conv_time[2]+lowtime);
}


function Put_hands()
{
	if (start_beginning > 0)
	{
		Put_sprite(13, 24, 180, 120, 66, hand_frames[hand_inst_anim]);
		Put_sprite(13, 224, 180, 120, 66, hand_frames[hand_inst_anim]);
		Put_sprite(13, 124, 96, 120, 66, hand_frames[hand_inst_anim]);
		Put_sprite(13, 124, 297, 120, 66, hand_frames[hand_inst_anim]);
		
		hand_inst_time++;
		if (hand_inst_time > 2)
		{
			hand_inst_anim++;
			if (hand_frames[hand_inst_anim] == 128) hand_inst_anim = 0;
			hand_inst_time = 0;
		}
	}
}

function Save_highscore()
{
	if (score > highscore)
	{
		highscore = score;
		Update_highscore();
		localStorage.setItem("killminds-highscore",parseInt(highscore));
	}
}

function Update_highscore()
{
	highscore_show[0] = ((highscore / 1000000) % 10) | 0;
	highscore_show[1] = ((highscore / 100000) % 10) | 0;
	highscore_show[2] = ((highscore / 10000) % 10) | 0;
	highscore_show[3] = ((highscore / 1000) % 10) | 0;
	highscore_show[4] = ((highscore / 100) % 10) | 0;
	highscore_show[5] = ((highscore / 10) % 10) | 0;
	highscore_show[6] = (highscore % 10) | 0;
}
