#include <iostream>
#include <cmath>
#include <cassert>
#include <ctime>
#include <iomanip>
#include "Game.hpp"

Game::Game() :
theme{
	{
		KColor::Black,
		KColor::Red,
		KColor::Blue,
		KColor::White,
		KColor::White,
		KColor::White
	}, {
		{ 25,  25,  25},
		{255, 100, 100},
		{ 80, 150, 255},
		{100, 100, 100},
		{255, 255, 255},
		{100, 100, 100}
	}, {
		{ 25,  25,  25},
		{150, 150, 150},
		{150, 150, 150},
		{100, 100, 100},
		{255, 255, 255},
		{120, 120, 120}
	}, {
		{110, 110, 110},
		{ 70,  70,  70},
		{ 70,  70,  70},
		{ 70,  70,  70},
		{ 50,  50,  50},
		{ 90,  90,  90}
	}, {
		{220, 220, 220},
		{140, 140, 140},
		{140, 140, 140},
		{140, 140, 140},
		{100, 100, 100},
		{180, 180, 180}
	}
},
setting{
	{"tile size",   8},
	{"fps",        60},
	{"width",      82},
	{"height",     50},
	{"ball speed", 80},
	{"ball size",   1},
	{"player 1 artificial intelligence", true},
	{"player 2 artificial intelligence", false},
	{"player 1 speed", 40},
	{"player 2 speed", 40},
	{"player 1 height", 8},
	{"player 2 height", 8},
	{"player 1 key up",   KKey::E},
	{"player 1 key down", KKey::D},
	{"player 2 key up",   KKey::Up},
	{"player 2 key down", KKey::Down},
	{"color theme", 1},
	{"color back",        KColor::Black},
	{"color player 1",    KColor::Red},
	{"color player 2",    KColor::Blue},
	{"color front",       KColor::White},
	{"color ball",        KColor::White},
	{"color middle line", KColor::White},
	{"server name", ""},
	{"membership address", {"ff13::526a:c6bb", 25588}},
	{"direct connect", {}}
} {
	// initialize state
	state = StatePlay;
	pause = true;
	catchedKey = KKey::None;

	// load from file eventual settings
	std::ifstream file("settings.txt");
	if (file.is_open())
	{
		setting.tile          .load(file, 1, 128);
		setting.width         .load(file, 1, 128);
		setting.fps           .load(file, 1, 1<<16);
		setting.height        .load(file, 1, 128);
		setting.ballSpeed     .load(file);
		setting.ballSize      .load(file, 1, 128);
		setting.player1Ai     .load(file);
		setting.player2Ai     .load(file);
		setting.player1Speed  .load(file);
		setting.player2Speed  .load(file);
		setting.player1Height .load(file, 1, 128);
		setting.player2Height .load(file, 1, 128);
		setting.player1KeyUp  .load(file);
		setting.player1KeyDown.load(file);
		setting.player2KeyUp  .load(file);
		setting.player2KeyDown.load(file);
		setting.theme         .load(file, 0, KTool::lenof(theme) - 1);
		setting.colorBack     .load(file);
		setting.colorPlayer1  .load(file);
		setting.colorPlayer2  .load(file);
		setting.colorFront    .load(file);
		setting.colorBall     .load(file);
		setting.colorMidLine  .load(file);
		setting.serverName    .load(file);
		setting.membershipAddr.load(file);
		setting.directConnect .load(file);
		file.close();
	}

	// affect custom color to theme 0
	theme[0].back    = setting.colorBack.value;
	theme[0].front   = setting.colorFront.value;
	theme[0].player1 = setting.colorPlayer1.value;
	theme[0].player2 = setting.colorPlayer2.value;
	theme[0].ball    = setting.colorBall.value;
	theme[0].midLine = setting.colorMidLine.value;

	// initialize dimensions
	tile   = setting.tile.value;
	width  = setting.width.value;
	height = setting.height.value;

	// initialize players
	player1.keyUp   = setting.player1KeyUp.value;
	player1.keyDown = setting.player1KeyDown.value;
	player2.keyUp   = setting.player2KeyUp.value;
	player2.keyDown = setting.player2KeyDown.value;
	player1.artificialIntelligence = setting.player1Ai.value;
	player2.artificialIntelligence = setting.player2Ai.value;

	// draw icon
	KSurface icon(64, 64, KColor(40, 40, 40));
	icon.fill({8, 8, 8, 28}, KColor(255, 80, 80));
	icon.fill({48, 28, 8, 28}, KColor(80, 180, 255));
	icon.fill({28, 28, 8, 8}, KColor::White);

	// open window
	window.open("Pong", width * tile, height * tile, icon);
	window.set_alpha_blend(true);
	colorFront = theme[setting.theme.value].front;
	colorBack  = theme[setting.theme.value].back;

	// initialize menu
	assert(font1.open("wendy.ttf", 2.5 * tile));
	assert(font2.open("wendy.ttf", 5 * tile));
	menu.page = 0;
	menu.line = 0;
	menu.generate(*this);
	server.menuCreate.line = 0;
	server.menuJoin.line = 0;

	// player 1
	player1.w = 1 * tile;
	player1.h = setting.player1Height.value * tile;
	player1.x = player1.w;
	player1.y = window.h / 2 - player1.h / 2;
	player1.color = theme[setting.theme.value].player1;
	player1.speed = setting.player1Speed.value / 1000.0 * tile;
	player1.score = 0;

	// player 2
	player2.w = 1 * tile;
	player2.h = setting.player2Height.value * tile;
	player2.x = window.w - 2 * player2.w;
	player2.y = window.h / 2 - player2.h / 2;
	player2.color = theme[setting.theme.value].player2;
	player2.speed = setting.player2Speed.value / 1000.0 * tile;
	player2.score = 0;

	scoreLeft  = font2("0", colorFront);
	scoreRight = font2("0", colorFront);

	// ball
	ball.w = setting.ballSize.value * tile;
	ball.h = setting.ballSize.value * tile;
	ball.color = theme[setting.theme.value].ball;
	ball.speed = setting.ballSpeed.value / 1000.0 * tile;
	reset_positions("left");

	update_collide_full();
	timer.start(16);
}

void Game::Menu::line_up()
{
	if (line > 0) line -= 1;
}

void Game::Menu::line_down()
{
	if (line + 1 < KTool::lenof(label[page]) and label[page][line + 1][0][0].size() > 0)
	{
		line += 1;
	}
}

void Game::Menu::generate(Game& game)
{
	Setting& setting = game.setting;
	w = 35 * game.tile;
	h = 43 * game.tile;
	x = game.window.w / 2 - w / 2;
	y = game.window.h / 2 - h / 2;
	title.assign("settings menu", game.font1, game.colorFront);

	// loop to set all settings label in menu
	for (unsigned i = 0; i < KTool::lenof(label); i++)
	{
		for (unsigned j  = 0; j < KTool::lenof(label[i]); j++)
		{
			label[i][j][0][0].set(game.colorBack);
			label[i][j][0][1].set(game.colorFront);
			label[i][j][1][0].set(game.colorBack);
			label[i][j][1][1].set(game.colorFront);
			label[i][j][0][0].set(game.font1);
			label[i][j][0][1].set(game.font1);
			label[i][j][1][0].set(game.font1);
			label[i][j][1][1].set(game.font1);
		}
	}

	// page 1
	label[0][PageNum	   ][0][0].change("page : 1/3");
	label[0][PageNum	   ][0][1].change("page : 1/3");
	label[0][Player1KeyUp  ][0][0].change("player 1 key up");
	label[0][Player1KeyUp  ][0][1].change("player 1 key up");
	label[0][Player1KeyUp  ][1][0].change(KKey::to_c_str(setting.player1KeyUp.value));
	label[0][Player1KeyUp  ][1][1].change(KKey::to_c_str(setting.player1KeyUp.value));
	label[0][Player1KeyDown][0][0].change("player 1 key down");
	label[0][Player1KeyDown][0][1].change("player 1 key down");
	label[0][Player1KeyDown][1][0].change(KKey::to_c_str(setting.player1KeyDown.value));
	label[0][Player1KeyDown][1][1].change(KKey::to_c_str(setting.player1KeyDown.value));
	label[0][Player2KeyUp  ][0][0].change("player 2 key up");
	label[0][Player2KeyUp  ][0][1].change("player 2 key up");
	label[0][Player2KeyUp  ][1][0].change(KKey::to_c_str(setting.player2KeyUp.value));
	label[0][Player2KeyUp  ][1][1].change(KKey::to_c_str(setting.player2KeyUp.value));
	label[0][Player2KeyDown][0][0].change("player 2 key down");
	label[0][Player2KeyDown][0][1].change("player 2 key down");
	label[0][Player2KeyDown][1][0].change(KKey::to_c_str(setting.player2KeyDown.value));
	label[0][Player2KeyDown][1][1].change(KKey::to_c_str(setting.player2KeyDown.value));
	label[0][Player1Ai     ][0][0].change("player 1 ai");
	label[0][Player1Ai     ][0][1].change("player 1 ai");
	label[0][Player1Ai     ][1][0].change(setting.player1Ai.value ? "true" : "false");
	label[0][Player1Ai     ][1][1].change(setting.player1Ai.value ? "true" : "false");
	label[0][Player2Ai     ][0][0].change("player 2 ai");
	label[0][Player2Ai     ][0][1].change("player 2 ai");
	label[0][Player2Ai     ][1][0].change(setting.player2Ai.value ? "true" : "false");
	label[0][Player2Ai     ][1][1].change(setting.player2Ai.value ? "true" : "false");
	label[0][Player1Speed  ][0][0].change("player 1 speed");
	label[0][Player1Speed  ][0][1].change("player 1 speed");
	label[0][Player1Speed  ][1][0].change(std::to_string(setting.player1Speed.value));
	label[0][Player1Speed  ][1][1].change(std::to_string(setting.player1Speed.value));
	label[0][Player2Speed  ][0][0].change("player 2 speed");
	label[0][Player2Speed  ][0][1].change("player 2 speed");
	label[0][Player2Speed  ][1][0].change(std::to_string(setting.player2Speed.value));
	label[0][Player2Speed  ][1][1].change(std::to_string(setting.player2Speed.value));
	label[0][BallSpeed     ][0][0].change("ball speed");
	label[0][BallSpeed     ][0][1].change("ball speed");
	label[0][BallSpeed     ][1][0].change(std::to_string(setting.ballSpeed.value));
	label[0][BallSpeed     ][1][1].change(std::to_string(setting.ballSpeed.value));

	// page 2
	label[1][PageNum   ][0][0].change("page : 2/3");
	label[1][PageNum   ][0][1].change("page : 2/3");
	label[1][ColorTheme][0][0].change("color theme");
	label[1][ColorTheme][0][1].change("color theme");
	label[1][ColorTheme][1][0].change(std::to_string(setting.theme.value));
	label[1][ColorTheme][1][1].change(std::to_string(setting.theme.value));
	label[1][TileSize  ][0][0].change("tile size");
	label[1][TileSize  ][0][1].change("tile size");
	label[1][TileSize  ][1][0].change(std::to_string(setting.tile.value));
	label[1][TileSize  ][1][1].change(std::to_string(setting.tile.value));
	label[1][Fps       ][0][0].change("fps");
	label[1][Fps       ][0][1].change("fps");
	label[1][Fps       ][1][0].change(std::to_string(setting.fps.value));
	label[1][Fps       ][1][1].change(std::to_string(setting.fps.value));
	label[1][Width     ][0][0].change("width");
	label[1][Width     ][0][1].change("width");
	label[1][Width     ][1][0].change(std::to_string(setting.width.value));
	label[1][Width     ][1][1].change(std::to_string(setting.width.value));
	label[1][Height    ][0][0].change("height");
	label[1][Height    ][0][1].change("height");
	label[1][Height    ][1][0].change(std::to_string(setting.height.value));
	label[1][Height    ][1][1].change(std::to_string(setting.height.value));
	label[1][Player1Height][0][0].change("player 1 height");
	label[1][Player1Height][0][1].change("player 1 height");
	label[1][Player1Height][1][0].change(std::to_string(setting.player1Height.value));
	label[1][Player1Height][1][1].change(std::to_string(setting.player1Height.value));
	label[1][Player2Height][0][0].change("player 2 height");
	label[1][Player2Height][0][1].change("player 2 height");
	label[1][Player2Height][1][0].change(std::to_string(setting.player2Height.value));
	label[1][Player2Height][1][1].change(std::to_string(setting.player2Height.value));

	// page 3
	label[2][PageNum     ][0][0].change("page : 3/3");
	label[2][PageNum     ][0][1].change("page : 3/3");
	label[2][Reset       ][0][0].change("reset");
	label[2][Reset       ][0][1].change("reset");
	label[2][CreateServer][0][0].change("create server");
	label[2][CreateServer][0][1].change("create server");
	label[2][JoinServer  ][0][0].change("join server");
	label[2][JoinServer  ][0][1].change("join server");
}

Game::~Game()
{
	// save settings
	std::ofstream file("settings.txt", std::ios::trunc);
	if (file.is_open())
	{
		setting.tile     .save(file, 10);
		setting.fps      .save(file, 10);
		setting.width    .save(file, 10);
		setting.height   .save(file, 10);
		setting.ballSpeed.save(file, 10);
		setting.ballSize .save(file, 10);
		setting.player1Ai.save(file);
		setting.player2Ai.save(file);
		setting.player1Speed .save(file, 15);
		setting.player2Speed .save(file, 15);
		setting.player1Height.save(file, 15);
		setting.player2Height.save(file, 15);
		file << std::endl;
		file << "# key list:" << std::endl;
		file << "# ├─ 0 1 2 3 4 5 6 7 8 9" << std::endl;
		file << "# ├─ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z" << std::endl;
		file << "# ├─ Left Right Up Down" << std::endl;
		file << "# ├─ Backspace Return Space Tab Escape" << std::endl;
		file << "# ├─ LShift LCtrl LAlt LBracket LMeta" << std::endl;
		file << "# ├─ RShift RCtrl RAlt RBracket RMeta" << std::endl;
		file << "# ├─ F1  F2  F3  F4  F5  F6  F7  F8  F9  F10 F11 F12" << std::endl;
		file << "# └─ F13 F14 F15 F16 F17 F18 F19 F20 F21 F22 F23 F24" << std::endl;
		setting.player1KeyUp  .save(file, 17);
		setting.player1KeyDown.save(file, 17);
		setting.player2KeyUp  .save(file, 17);
		setting.player2KeyDown.save(file, 17);
		file << std::endl << "# color theme list:" << std::endl;
		file << "# └─ 0 1 2 3 4" << std::endl;
		setting.theme.save(file);
		file << std::endl << "# color format, 0 ≤ N ≤ 255:" << std::endl;
		file << "# ├─ N N N N" << std::endl;
		file << "# │  or (last N is alpha and is optional)" << std::endl;
		file << "# └─ N N N" << std::endl;
		file << "# these settings only affect theme 0:" << std::endl;
		setting.colorBack   .save(file, 17);
		setting.colorFront  .save(file, 17);
		setting.colorPlayer1.save(file, 17);
		setting.colorPlayer2.save(file, 17);
		setting.colorBall   .save(file, 17);
		setting.colorMidLine.save(file, 17);
		file << std::endl << "# server configuration" << std::endl;
		setting.serverName.save(file);
		setting.membershipAddr.save(file);
		setting.directConnect.save(file);
		file.close();
	}
}

void Game::draw_background()
{
	window.fill(colorBack);
}

void Game::draw_game()
{
	// score
	window.print(scoreLeft,      window.w / 4 -  scoreLeft.w / 2, window.h / 8);
	window.print(scoreRight, 3 * window.w / 4 - scoreRight.w / 2, window.h / 8);

	// middle line
	for (int y = 0; y < window.h; y += tile)
	{
		window.fill({window.w / 2 - tile / 2, y, tile, tile / 2}, theme[setting.theme.value].midLine);
	}

	// players
	window.fill(player1, player1.color);
	window.fill(player2, player2.color);

	// ball
	window.fill(ball, ball.color);
}

void Game::draw_menu()
{
	menu.print(window, colorFront, colorBack, tile);
}

void Game::draw_menu_server_create()
{
	server.menuCreate.print(window, colorFront, colorBack, tile);
}
void Game::draw_menu_server_list()
{
	server.menuJoin.print(window, colorFront, colorBack, tile);
}

void Game::draw_menu_server_info()
{
	server.menuInfo.print(window, colorFront, colorBack, tile);
}

void Game::update_display()
{
	window.display();
}

void Game::player_update_movement(KControl& control, Player& player, int time)
{
	enum {Up, None, Down} choice = None;
	// if player is going up
	if (control[player.keyUp] and not control[player.keyDown]) choice = Up;
	// if player is going down
	else if (control[player.keyDown] and not control[player.keyUp]) choice = Down;
	// if artificial intelligence is active
	else if (player.artificialIntelligence)
	{
		// by default, the player as to be in the middle of the screen
		float pos = window.h / 2;
		// avoid error by checking forbiden value
		if (ball.vx != 0 and window.h > ball.h)
		{
			// if the ball comes to the player
			if (ball.vx > 0 ? (ball.x + ball.w < player.x) : (player.x + player.w < ball.x))
			{
				// process the distance between the player and the ball
				float distance = ball.vx > 0 ? (player.x - ball.x - ball.w) : (player.x + player.w - ball.x);
				// process the vertical distance that the ball will travel
				float vertical = ball.vy / ball.vx * distance + ball.y;
				// report this distance on the screen range
				// use floating modulo
				pos = std::abs(std::fmod(vertical - ball.h / 2, 2 * (window.h - ball.h)) + ball.h / 2);
				if (pos > window.h - ball.h / 2)
				{
					pos = 2 * (window.h - ball.h) - pos;
				}
				// std::cout << "\rprediction : " << pos << std::flush;
				pos += ball.h / 2;
			}
		}
		// if player is under the ball
		if (player.y + player.h / 2 > pos)
		{
			if (player.y + player.h - player.speed * time / 2 > pos) choice = Up;
		}
		else
		{
			if (player.y + player.h / 2 + player.speed * time < pos) choice = Down;
		}
	}
	if (choice == Up)
	{
		player.y -= player.speed * time;
		// if the raquet goes over the screen
		if (player.y < 0) {
			player.y = 0;
		}
		// if the raquet overlap the ball, it push it
		if (player.is_on(ball))
		{
			ball.y = player.y + player.h;
		}
	}
	else if (choice == Down)
	{
		player.y += player.speed * time;
		// if the raquet goes over the screen
		if (player.y + player.h > window.h)
		{
			player.y = window.h - player.h;
		}
		// if the raquet overlap the ball, it push it
		if (player.is_on(ball))
		{
			ball.y = player.y - ball.h;
		}
	}
}

void Game::ball_update_movement() {
	int time = timer.ms;
	// update colliders
	collide[CollidePlayer1Front ].y = player1.y;
	collide[CollidePlayer2Front ].y = player2.y;
	collide[CollidePlayer1Top   ].y = player1.y + player1.h;
	collide[CollidePlayer2Top   ].y = player2.y + player2.h;
	collide[CollidePlayer1Bottom].y = player1.y;
	collide[CollidePlayer2Bottom].y = player2.y;

	// manage ball collision
	// create iterators of KCollide::Border at end of 'collide'
	auto *end = collide + CollidesAmount;
	// this iterator will point to the border touched by the ball
	auto *selected = end;
	// the ratio represent the percentage of the ball vector that it manage to
	// travel before touching any border
	float ratio = 1;
	float traveled;
	// 'loop' will be set to 'false' if no collision occured
	bool loop = true;
	// if the ball is stuck between two borders, a security count will prevent it
	// from bouncing indefinitly
	int securityCount = 64;
	while (loop and securityCount > 0)
	{
		securityCount--;
		// use KCollide::find() to detect any collision between the ball and a border
		// the new ratio is the remaining ratio (ramining percentage of the vector to travel)
		traveled = ratio * KCollide::find(ball, ball.vx * time * ratio, ball.vy * time * ratio, collide, end, selected);
		ball.x += ball.vx * time * traveled;
		ball.y += ball.vy * time * traveled;
		ratio -= traveled;
		// if a collision occured
		if (selected != end)
		{
			// the ball has to react depending on what it touched
			switch (selected->id) {
				// if a point is marqued
				case CollideLeft:
				player2.score++;
				scoreRight = font2(std::to_string(player2.score), colorFront);
				reset_positions("left");
				pause = true;
				loop = false;
				break;
				case CollideRight:
				player1.score++;
				scoreLeft = font2(std::to_string(player1.score), colorFront);
				reset_positions("right");
				pause = true;
				loop = false;
				break;
				// if it bounces horizontally
				case CollideTop:
				case CollideBottom:
				case CollidePlayer1Top:
				case CollidePlayer2Top:
				case CollidePlayer1Bottom:
				case CollidePlayer2Bottom:
				ball.bounce("horizontally");
				break;
				// if it bounces vertically
				case CollidePlayer1Front:
				ball.bounce("vertically");
				ball.deflect(player1);
				break;
				case CollidePlayer2Front:
				ball.bounce("vertically");
				ball.deflect(player2);
			}
		}
		// if no collision occured
		else
		{
			loop = false;
		}
	}

	// if the ball is not on the screen
	if (not ball.is_on(window)) {
		KRect<> rect = {
			window.x - 6 * tile,
			window.y,
			window.w + 12 * tile,
			window.h
		};
		if (not ball.is_on(rect)) {
			reset_positions(ball.vx > 0 ? "right" : "left");
			pause = true;
		}
	}

	// if ball is blocked
	if (securityCount == 0)
	{
		reset_positions(ball.vx > 0 ? "right" : "left");
		pause = true;
	}
}

void Game::Ball::update_vector()
{
	// when the angle is changed, the vector has to be updated
	vx = std::cos(angle) * speed;
	vy = std::sin(angle) * speed;
}

void Game::Ball::bounce(const char* direction)
{
	// direction can be horizontal or vertical
	if (direction[0] == 'v') angle -= pi;
	angle = -angle;
	update_vector();
}

void Game::Ball::reset(float x, float y, const char* direction)
{
	// direction can be left or right
	angle = (direction[0] == 'l') ? pi : 0;
	this->x = x - w / 2;
	this->y = y - h / 2;
	update_vector();
}

void Game::reset_positions(const char* direction)
{
	ball.reset(window.w / 2, window.h / 2, direction);
	player1.y = window.h / 2 - player1.h / 2;
	player2.y = window.h / 2 - player2.h / 2;
}

void Game::Menu::print(KWindow& window, KColor cb, KColor cf, int t)
{
	window.fill({x, y, w, h}, cb);
	KRect r(x + t, y + t, w - 2 * t, 5 * t);
	int th = title.h;
	window.fill(r, cf);
	window.print(title, r.x + r.w / 2 - title.w / 2, r.y + r.h / 2 - th / 2);

	r.h = 3 * t;
	r.y += 6 * t;
	if (line == 0) window.fill(r, cf);
	window.print(label[page][0][0][int(line == 0)], r.x + t, r.y + r.h / 2 - th / 2);
	r.y += 4 * t;
	for (unsigned m = 1; m < KTool::lenof(label[page]) and label[page][m][0][0].size() > 0; m++)
	{
		if (line == m) window.fill(r, cf);
		window.print(label[page][m][0][int(line == m)], r.x + t, r.y + r.h / 2 - th / 2);
		window.print(label[page][m][1][int(line == m)], r.x + r.w - label[page][m][1][int(line == m)].w - t, r.y + r.h / 2 - th / 2);
		r.y += 3 * t;
	}
}

void Game::Ball::deflect(Player& player)
{
	// factor is a relative position factor of where the ball touched the raquet
	// range : {-1, 1}, 0 mean in the middle.
	float factor = (y - player.y - player.h) * 2 / (player.h + h) + 1;

	if (factor < 1.0/16 and factor > -1.0/16) return;

	// calculations depends on ball direction
	if (vx > 0)
	{
		angle = std::atan(std::tan(angle) + factor * 0.625);
		float tangent = std::tan(angle);
		// the deflection if roofed by an angle of 45 degrees
		if (tangent < -1) {
			angle = 1.75 * pi;
		}
		else if (tangent > 1) {
			angle = 0.25 * pi;
		}
	}
	else
	{
		angle = std::atan(std::tan(angle) - factor * 0.625) + pi;
		float tangent = std::tan(angle);
		if (tangent < -1) {
			angle = 0.75 * pi;
		}
		else if (tangent > 1) {
			angle = 1.25 * pi;
		}
	}
	update_vector();
}

void Game::update_collide_full() {
	collide[CollideLeft         ].assign(KCollide::Left, window.x - 6 * tile, window.y, window.h, CollideLeft);
	collide[CollideRight        ].assign(KCollide::Right, window.w + 6 * tile, window.y, window.h, CollideRight);
	collide[CollideTop          ].assign(KCollide::Up, window.x - 6 * tile, window.y, window.w + 12 * tile, CollideTop);
	collide[CollideBottom       ].assign(KCollide::Down, window.x - 6 * tile, window.h, window.w + 12 * tile, CollideBottom);
	collide[CollidePlayer1Front ].assign(KCollide::Left, player1.x + player1.w, player1.y, player1.h, CollidePlayer1Front);
	collide[CollidePlayer2Front ].assign(KCollide::Right, player2.x, player2.y, player2.h, CollidePlayer2Front);
	collide[CollidePlayer1Top   ].assign(KCollide::Down, player1.x, player1.y, player1.w, Game::CollidePlayer1Top);
	collide[CollidePlayer2Top   ].assign(KCollide::Down, player2.x, player2.y, player2.w, CollidePlayer2Top);
	collide[CollidePlayer1Bottom].assign(KCollide::Up, player1.x, player1.y + player1.h, player1.w, CollidePlayer1Bottom);
	collide[CollidePlayer2Bottom].assign(KCollide::Up, player2.x, player2.y + player2.h, player2.w, CollidePlayer2Bottom);
}

void Game::Server::MenuCreate::generate(Game& game) {
	w = 35 * game.tile;
	h = 43 * game.tile;
	x = game.window.w / 2 - w / 2;
	y = game.window.h / 2 - h / 2;
	title.assign("create server", game.font1, game.colorFront);
	for (unsigned i = 0; i < KTool::lenof(label); i++) {
		label[i][0][0].set(game.colorBack);
		label[i][0][1].set(game.colorFront);
		label[i][1][0].set(game.colorBack);
		label[i][1][1].set(game.colorFront);
		label[i][0][0].set(game.font1);
		label[i][0][1].set(game.font1);
		label[i][1][0].set(game.font1);
		label[i][1][1].set(game.font1);
	}
	label[BackToMenu   ][0][0].change("back to menu");
	label[BackToMenu   ][0][1].change("back to menu");
	label[ServerName   ][0][0].change("server name");
	label[ServerName   ][0][1].change("server name");
	label[ServerName   ][1][0].change(game.setting.serverName.value);
	label[ServerName   ][1][1].change(game.setting.serverName.value);
	label[Width        ][0][0].change("width");
	label[Width        ][0][1].change("width");
	label[Width        ][1][0].change(std::to_string((int)game.server.config.width));
	label[Width        ][1][1].change(std::to_string((int)game.server.config.width));
	label[Height       ][0][0].change("height");
	label[Height       ][0][1].change("height");
	label[Height       ][1][0].change(std::to_string((int)game.server.config.height));
	label[Height       ][1][1].change(std::to_string((int)game.server.config.height));
	label[Player1Height][0][0].change("player 1 height");
	label[Player1Height][0][1].change("player 1 height");
	label[Player1Height][1][0].change(std::to_string((int)game.server.config.player1Height));
	label[Player1Height][1][1].change(std::to_string((int)game.server.config.player1Height));
	label[Player2Height][0][0].change("player 2 height");
	label[Player2Height][0][1].change("player 2 height");
	label[Player2Height][1][0].change(std::to_string((int)game.server.config.player2Height));
	label[Player2Height][1][1].change(std::to_string((int)game.server.config.player2Height));
	label[Player1Speed ][0][0].change("player 1 speed");
	label[Player1Speed ][0][1].change("player 1 speed");
	label[Player1Speed ][1][0].change(std::to_string((int)game.server.config.player1Speed));
	label[Player1Speed ][1][1].change(std::to_string((int)game.server.config.player1Speed));
	label[Player2Speed ][0][0].change("player 2 speed");
	label[Player2Speed ][0][1].change("player 2 speed");
	label[Player2Speed ][1][0].change(std::to_string((int)game.server.config.player2Speed));
	label[Player2Speed ][1][1].change(std::to_string((int)game.server.config.player2Speed));
	label[BallSize     ][0][0].change("ball size");
	label[BallSize     ][0][1].change("ball size");
	label[BallSize     ][1][0].change(std::to_string((int)game.server.config.ballSize));
	label[BallSize     ][1][1].change(std::to_string((int)game.server.config.ballSize));
	label[BallSpeed    ][0][0].change("ball speed");
	label[BallSpeed    ][0][1].change("ball speed");
	label[BallSpeed    ][1][0].change(std::to_string((int)game.server.config.ballSpeed));
	label[BallSpeed    ][1][1].change(std::to_string((int)game.server.config.ballSpeed));
}

void Game::Server::MenuInfo::generate(Game& game) {
	w = 35 * game.tile;
	h = 43 * game.tile;
	x = game.window.w / 2 - w / 2;
	y = game.window.h / 2 - h / 2;
	line = 0;
	title.assign("server info", game.font1, game.colorFront);
	for (unsigned i = 0; i < KTool::lenof(label); i++) {
		label[i][0][0].set(game.colorBack);
		label[i][0][1].set(game.colorFront);
		label[i][1][0].set(game.colorBack);
		label[i][1][1].set(game.colorFront);
		label[i][0][0].set(game.font1);
		label[i][0][1].set(game.font1);
		label[i][1][0].set(game.font1);
		label[i][1][1].set(game.font1);
	}
	label[BackToMenu   ][0][0].change("back to menu");
	label[BackToMenu   ][0][1].change("back to menu");
	label[Join         ][0][0].change("join");
	label[Join         ][0][1].change("join");
	label[ServerName   ][0][0].change("server name");
	label[ServerName   ][0][1].change("server name");
	label[ServerName   ][1][0].change(std::string(game.server.config.name, sizeof game.server.config.name));
	label[ServerName   ][1][1].change(std::string(game.server.config.name, sizeof game.server.config.name));
	label[Width        ][0][0].change("width");
	label[Width        ][0][1].change("width");
	label[Width        ][1][0].change(std::to_string((int)game.server.config.width));
	label[Width        ][1][1].change(std::to_string((int)game.server.config.width));
	label[Height       ][0][0].change("height");
	label[Height       ][0][1].change("height");
	label[Height       ][1][0].change(std::to_string((int)game.server.config.height));
	label[Height       ][1][1].change(std::to_string((int)game.server.config.height));
	label[Player1Height][0][0].change("player 1 height");
	label[Player1Height][0][1].change("player 1 height");
	label[Player1Height][1][0].change(std::to_string((int)game.server.config.player1Height));
	label[Player1Height][1][1].change(std::to_string((int)game.server.config.player1Height));
	label[Player2Height][0][0].change("player 2 height");
	label[Player2Height][0][1].change("player 2 height");
	label[Player2Height][1][0].change(std::to_string((int)game.server.config.player2Height));
	label[Player2Height][1][1].change(std::to_string((int)game.server.config.player2Height));
	label[Player1Speed ][0][0].change("player 1 speed");
	label[Player1Speed ][0][1].change("player 1 speed");
	label[Player1Speed ][1][0].change(std::to_string((int)game.server.config.player1Speed));
	label[Player1Speed ][1][1].change(std::to_string((int)game.server.config.player1Speed));
	label[Player2Speed ][0][0].change("player 2 speed");
	label[Player2Speed ][0][1].change("player 2 speed");
	label[Player2Speed ][1][0].change(std::to_string((int)game.server.config.player2Speed));
	label[Player2Speed ][1][1].change(std::to_string((int)game.server.config.player2Speed));
	label[BallSize     ][0][0].change("ball size");
	label[BallSize     ][0][1].change("ball size");
	label[BallSize     ][1][0].change(std::to_string((int)game.server.config.ballSize));
	label[BallSize     ][1][1].change(std::to_string((int)game.server.config.ballSize));
	label[BallSpeed    ][0][0].change("ball speed");
	label[BallSpeed    ][0][1].change("ball speed");
	label[BallSpeed    ][1][0].change(std::to_string((int)game.server.config.ballSpeed));
	label[BallSpeed    ][1][1].change(std::to_string((int)game.server.config.ballSpeed));
}

void Game::Server::MenuJoin::generate(Game& game) {
	w = 35 * game.tile;
	h = 43 * game.tile;
	x = game.window.w / 2 - w / 2;
	y = game.window.h / 2 - h / 2;
	title.assign("join server", game.font1, game.colorFront);
	for (unsigned i = 0; i < KTool::lenof(label); i++) {
		label[i][0][0].set(game.colorBack);
		label[i][0][1].set(game.colorFront);
		label[i][1][0].set(game.colorBack);
		label[i][1][1].set(game.colorFront);
		label[i][2][0].set(game.colorBack);
		label[i][2][1].set(game.colorFront);
		label[i][0][0].set(game.font1);
		label[i][0][1].set(game.font1);
		label[i][1][0].set(game.font1);
		label[i][1][1].set(game.font1);
		label[i][2][0].set(game.font1);
		label[i][2][1].set(game.font1);
	}
	label[0][0][0].change("back to menu");
	label[0][0][1].change("back to menu");
	generate_server_list(game);
}

void Game::Server::MenuJoin::generate_server_list(Game& game) {
	for (unsigned i = 0; i + 1 < KTool::lenof(label); i++) {
		if (i < game.server.availableServers.size()) {
			Game::Server::Config& config = game.server.availableServers[i].config;
			std::string buffer(config.name, KTool::lenof(config.name));
			label[i + 1][0][0].change(buffer);
			label[i + 1][0][1].change(buffer);
			if (
				config.width         == 82 &&
				config.height        == 50 &&
				config.player1Height ==  8 &&
				config.player2Height ==  8 &&
				config.player1Speed  == 40 &&
				config.player2Speed  == 40 &&
				config.ballSize      ==  1 &&
				config.ballSpeed     == 80
			) {
				buffer = "default";
			}
			else if (
				config.player1Height == config.player2Height &&
				config.player1Speed  == config.player2Speed
			) {
				buffer = "symetric";
			}
			else {
				buffer = "asymetric";
			}
			label[i + 1][1][0].change(buffer);
			label[i + 1][1][1].change(buffer);
			buffer = std::to_string(config.width) + ":" + std::to_string(config.height);
			label[i + 1][2][0].change(buffer);
			label[i + 1][2][1].change(buffer);
		}
		else {
			label[i + 1][0][0].change("");
			label[i + 1][0][1].change("");
			label[i + 1][1][0].change("");
			label[i + 1][1][1].change("");
			label[i + 1][2][0].change("");
			label[i + 1][2][1].change("");
		}
	}
}

void Game::Server::MenuCreate::print(KWindow& window, KColor cb, KColor cf, int t) {
	window.fill({x, y, w, h}, cb);
	KRect r(x + t, y + t, w - 2 * t, 5 * t);
	int th = title.h;
	window.fill(r, cf);
	window.print(title, r.x + r.w / 2 - title.w / 2, r.y + r.h / 2 - th / 2);

	r.h = 3 * t;
	r.y += 6 * t;
	if (line == 0) window.fill(r, cf);
	window.print(label[0][0][int(line == 0)], r.x + t, r.y + r.h / 2 - th / 2);
	r.y += 4 * t;
	for (unsigned m = 1; m < KTool::lenof(label) and label[m][0][0].size() > 0; m++)
	{
		if (line == m) window.fill(r, cf);
		window.print(label[m][0][int(line == m)], r.x + t, r.y + r.h / 2 - th / 2);
		window.print(label[m][1][int(line == m)], r.x + r.w - label[m][1][0].w - t, r.y + r.h / 2 - th / 2);
		r.y += 3 * t;
	}
}

void Game::Server::MenuInfo::print(KWindow& window, KColor cb, KColor cf, int t) {
	window.fill({x, y, w, h}, cb);
	KRect r(x + t, y + t, w - 2 * t, 5 * t);
	int th = title.h;
	window.fill(r, cf);
	window.print(title, r.x + r.w / 2 - title.w / 2, r.y + r.h / 2 - th / 2);

	r.h = 3 * t;
	r.y += 6 * t;
	if (line == BackToMenu) window.fill(r, cf);
	window.print(label[BackToMenu][0][int(line == BackToMenu)], r.x + t, r.y + r.h / 2 - th / 2);
	r.y += 3 * t;
	if (line == Join) window.fill(r, cf);
	window.print(label[Join][0][int(line == Join)], r.x + t, r.y + r.h / 2 - th / 2);
	r.y += 4 * t;
	for (unsigned m = 2; m < KTool::lenof(label) and label[m][0][0].size() > 0; m++)
	{
		if (line == m) window.fill(r, cf);
		window.print(label[m][0][int(line == m)], r.x + t, r.y + r.h / 2 - th / 2);
		window.print(label[m][1][int(line == m)], r.x + r.w - label[m][1][0].w - t, r.y + r.h / 2 - th / 2);
		r.y += 3 * t;
	}
}

void Game::Server::MenuJoin::print(KWindow& window, KColor cb, KColor cf, int t) {
	window.fill({x, y, w, h}, cb);
	KRect r(x + t, y + t, w - 2 * t, 5 * t);
	int th = title.h;
	window.fill(r, cf);
	window.print(title, r.x + r.w / 2 - title.w / 2, r.y + r.h / 2 - th / 2);

	r.h = 3 * t;
	r.y += 6 * t;
	if (line == 0) window.fill(r, cf);
	window.print(label[0][0][int(line == 0)], r.x + t, r.y + r.h / 2 - th / 2);
	r.y += 4 * t;
	for (unsigned m = 1; m < KTool::lenof(label); m++)
	{
		if (line == m) window.fill(r, cf);
		window.print(label[m][0][int(line == m)], r.x + t, r.y + r.h / 2 - th / 2);
		window.print(label[m][1][int(line == m)], x + w / 2 - label[m][1][0].w / 2, r.y + r.h / 2 - th / 2);
		window.print(label[m][2][int(line == m)], r.x + r.w - label[m][2][0].w - t, r.y + r.h / 2 - th / 2);
		r.y += 3 * t;
	}
}

void Game::state_update_serverList() {
	bool change = false;
	if (server.lastUpdateTime < std::time(nullptr)) {
		server.lastUpdateTime = std::time(nullptr);
		uint8_t flag = 0;
		if (not server.socket.write_var(setting.membershipAddr.value, flag)) {
			std::ofstream logFile("log.txt", std::ios::app);
			logFile << KTimer::date() << " [ERROR] " << server.socket.error << std::endl;
		}
		auto iter = server.availableServers.begin();
		while (iter != server.availableServers.end()) {
			if (iter->updateTime + 3 < server.lastUpdateTime) {
				server.availableServers.erase(iter);
				change = true;
			}
			else {
				iter += 1;
			}
		}
	}
	KSocket::Addr address;
	while (server.socket.read_var(address, server.config)) {
		unsigned i = 0;
		while (
			i < server.availableServers.size() and
			address != server.availableServers[i].address
		) {
			i += 1;
		}
		if (i == server.availableServers.size()) {
			server.availableServers.push_back({std::time(nullptr), address, server.config});
			change = true;
		}
		else {
			if (memcmp(&server.availableServers[i].config, &server.config, sizeof server.config) != 0) {
				server.availableServers[i] = Server::AvailableServer{std::time(nullptr), address, server.config};
				change = true;
			}
			else {
				server.availableServers[i].updateTime = std::time(nullptr);
			}
		}
	}
	if (change) {
		server.menuJoin.generate_server_list(*this);
	}
}

void Game::state_update_serverCreate() {
	uint8_t buffer[128];
	KSocket::Addr address;
	while (server.socket.read(server.peerAddr, buffer, sizeof buffer)) {
		if (buffer[0] == 0) {
			server.socket.write_var(server.peerAddr, server.config);
		}
		else {
			state_switch_serverCreate_to_playServer();
			return;
		}
	}
}

void Game::Server::fill_state_from_game(Game& game) {
	if (package.flag & package.PlayerPos) {
		state.playerPos = (double)game.player2.y / game.window.h * 0x10000;
	}
	if (package.flag & package.PlayerMov) {
		if (game.control[game.player2.keyUp] and not game.control[game.player2.keyDown]) {
			state.playerMov = state.Up;
		}
		else if (game.control[game.player2.keyDown] and not game.control[game.player2.keyUp]) {
			state.playerMov = state.Down;
		}
		else {
			state.playerMov = state.Static;
		}
	}
	if (package.flag & package.BallState) {
		if (game.ball.angle < 0 or game.ball.angle >= 2 * game.ball.pi) {
			game.ball.angle = std::fmod(game.ball.angle, 2 * game.ball.pi);
		}
		if (isClient) {
			state.ballRad = ((double)game.ball.pi - game.ball.angle) / (2 * game.ball.pi) * 0x10000;
			state.ballPosX = ((double)(game.window.w + 12 * game.tile) - (game.ball.x + 6 * game.tile) - game.ball.w) / (game.window.w + 12 * game.tile) * 0x10000;
			state.ballPosY = (double)game.ball.y / game.window.h * 0x10000;
		}
		else {
			state.ballRad = (double)game.ball.angle / (2 * game.ball.pi) * 0x10000;
			state.ballPosX = (double)(game.ball.x + 6 * game.tile) / (game.window.w + 12 * game.tile) * 0x10000;
			state.ballPosY = (double)game.ball.y / game.window.h * 0x10000;
		}
	}
	if (not isClient) {
		if (game.ball.vx < 0) {
			package.flag |= package.Handle;
		}
	}
}

void Game::Server::apply_state_to_game(Game& game) {
	if (isClient) {
		if (package.flag & package.Handle) {
			state.graph = state.Handle;
		}
		else {
			state.graph = state.Passive;
		}
	}
	else {
		if (game.ball.vx > 0) {
			state.graph = state.Handle;
		}
		else {
			state.graph = state.Passive;
		}
	}
	if (package.flag & package.Point) {
		game.player2.score++;
		game.scoreRight = game.font2(std::to_string(game.player2.score), game.colorFront);
		game.reset_positions("left");
	}
	game.pause = not state.play;
	if (package.flag & package.PlayerPos) {
		game.player1.y = (double)state.playerPos / 0x10000 * game.window.h;
	}
	if (state.graph == state.Passive && (package.flag & package.BallState)) {
		game.ball.angle = double((uintmax_t)state.ballRad * 2 * game.ball.pi) / 0x10000;
		game.ball.x = double((uintmax_t)state.ballPosX * (game.window.w + 12 * game.tile)) / 0x10000 - 6 * game.tile;
		game.ball.y = double((uintmax_t)state.ballPosY * game.window.h) / 0x10000;
		if (isClient) {
			game.ball.angle = game.ball.pi - game.ball.angle;
			game.ball.x = game.window.w - game.ball.x - game.ball.w;
		}
		game.ball.update_vector();
	}
}

void Game::Server::get_peer_packages() {
	uint8_t buffer[128];
	unsigned size;
	KSocket::Addr address;
	while (socket.read(address, (void*)&buffer[0], sizeof buffer)) {
		if (address == peerAddr) {
			size = 1;
			lastUpdateTime = std::time(nullptr);
			package.flag = buffer[0];
			if (buffer[0] & package.PlayerMov) {
				state.playerMov = buffer[size++];
			}
			if (buffer[0] & package.PlayerPos) {
				state.playerPos = buffer[size++] * 0x100;
				state.playerPos += buffer[size++];
			}
			if (buffer[0] & package.BallState) {
				state.ballRad   = buffer[size++] * 0x100;
				state.ballRad  += buffer[size++];
				state.ballPosX  = buffer[size++] * 0x100;
				state.ballPosX += buffer[size++];
				state.ballPosY  = buffer[size++] * 0x100;
				state.ballPosY += buffer[size++];
				// if (isClient == true) {
				// 	std::cout << "received pos : " << (unsigned)state.ballPosX << std::endl;
				// }
			}
			if (buffer[0] & package.Pause) {
				state.play = false;
			}
			if (buffer[0] & package.Play) {
				state.play = true;
			}
			if (buffer[0] & package.Quit) {
				state.quit = true;
			}
		}
	}
}

void Game::Server::send_package_to_peer() {
	unsigned size = 1;
	uint8_t buffer[128];
	buffer[0] = package.flag;
	if (buffer[0] & package.PlayerMov) {
		buffer[size++] = state.playerMov;
	}
	if (buffer[0] & package.PlayerPos) {
		buffer[size++] = (state.playerPos & 0xff00) / 0x100;
		buffer[size++] = state.playerPos & 0x00ff;
	}
	if (buffer[0] & package.BallState) {
		// if (isClient == false) {
		// 	std::cout << "pos var : " << (unsigned)state.ballPosX << std::endl;
		// }
		buffer[size++] = (state.ballRad & 0xff00) / 0x100;
		buffer[size++] = state.ballRad & 0x00ff;
		buffer[size++] = (state.ballPosX & 0xff00) / 0x100;
		buffer[size++] = state.ballPosX & 0x00ff;
		buffer[size++] = (state.ballPosY & 0xff00) / 0x100;
		buffer[size++] = state.ballPosY & 0x00ff;
	}
	socket.write(peerAddr, (void*)&buffer[0], size);

	package.flag = 0;
}
