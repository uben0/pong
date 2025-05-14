#include "Game.hpp"

void Game::Setting::reset_all(Game& game)
{
	tile.value = 8;
	update_tile(game);
	fps.value = 60;
	update_fps(game);
	width.value = 82;
	update_width(game);
	height.value = 50;
	update_height(game);
	ballSpeed.value = 80;
	update_ballSpeed(game);
	ballSize.value = 1;
	update_ballSize(game);
	player1Ai.value = true;
	update_player1Ai(game);
	player2Ai.value = false;
	update_player2Ai(game);
	player1Speed.value = 40;
	update_player1Speed(game);
	player2Speed.value = 40;
	update_player2Speed(game);
	player1Height.value = 8;
	update_player1Height(game);
	player2Height.value = 8;
	update_player2Height(game);
	player1KeyUp.value = KKey::E;
	update_player1KeyUp(game);
	player1KeyDown.value = KKey::D;
	update_player1KeyDown(game);
	player2KeyUp.value = KKey::Up;
	update_player2KeyUp(game);
	player2KeyDown.value = KKey::Down;
	update_player2KeyDown(game);
	theme.value = 1;
	colorBack   .value = KColor::Black;
	colorPlayer1.value = KColor::Red;
	colorPlayer2.value = KColor::Blue;
	colorFront  .value = KColor::White;
	colorBall   .value = KColor::White;
	colorMidLine.value = KColor::White;
	update_theme(game);
}

void Game::Setting::update_tile(Game& game)
{
	game.tile = tile.value;
	game.window.resize(game.width * game.tile, game.height * game.tile);

	assert(game.font1.open("wendy.ttf", 2.5 * game.tile));
	assert(game.font2.open("wendy.ttf", 5 * game.tile));
	game.menu.generate(game);

	// player 1
	game.player1.w = 1 * game.tile;
	game.player1.h = player1Height.value * game.tile;
	game.player1.x = game.player1.w;
	game.player1.speed = player1Speed.value / 1000.0 * game.tile;

	// player 2
	game.player2.w = 1 * game.tile;
	game.player2.h = player2Height.value * game.tile;
	game.player2.x = game.window.w - 2 * game.player2.w;
	game.player2.speed = player2Speed.value / 1000.0 * game.tile;

	game.scoreLeft  = game.font2(std::to_string(game.player1.score), game.colorFront);
	game.scoreRight = game.font2(std::to_string(game.player2.score), game.colorFront);

	// ball
	game.ball.w = ballSize.value * game.tile;
	game.ball.h = ballSize.value * game.tile;
	game.ball.speed = ballSpeed.value / 1000.0 * game.tile;

	game.reset_positions("left");
	game.update_collide_full();
}

void Game::Setting::update_width(Game& game)
{
	game.width = width.value;
	game.window.resize(game.width * game.tile, game.height * game.tile);
	game.menu.x = game.window.w / 2 - game.menu.w / 2;
	game.player2.x = game.window.w - 2 * game.tile;
	game.reset_positions("left");
	game.update_collide_full();
}

void Game::Setting::update_height(Game& game)
{
	game.height = height.value;
	game.window.resize(game.width * game.tile, game.height * game.tile);
	game.menu.y = game.window.h / 2 - game.menu.h / 2;
	game.reset_positions("left");
	game.update_collide_full();
}

void Game::Setting::update_ballSpeed(Game& game)
{
	game.ball.speed = ballSpeed.value / 1000.0 * game.tile;
	game.ball.update_vector();
}

void Game::Setting::update_ballSize(Game& game)
{
	game.ball.w = ballSize.value * game.tile;
	game.ball.h = ballSize.value * game.tile;
}

void Game::Setting::update_player1Ai(Game& game)
{
	game.player1.artificialIntelligence = player1Ai.value;
}

void Game::Setting::update_player2Ai(Game& game)
{
	game.player2.artificialIntelligence = player2Ai.value;
}

void Game::Setting::update_player1Speed(Game& game)
{
	game.player1.speed = player1Speed.value / 1000.0 * game.tile;
}

void Game::Setting::update_player2Speed(Game& game)
{
	game.player2.speed = player2Speed.value / 1000.0 * game.tile;
}

void Game::Setting::update_player1Height(Game& game)
{
	int middle = game.player1.y + game.player1.h / 2;
	game.player1.h = player1Height.value * game.tile;
	game.player1.y = middle - game.player1.h / 2;
	if (game.player1.y < 0) game.player1.y = 0;
	if (game.player1.y + game.player1.h > game.window.h)
	{
		game.player1.y = game.window.h - game.player1.h;
	}
	game.update_collide_full();
}

void Game::Setting::update_player2Height(Game& game)
{
	int middle = game.player2.y + game.player2.h / 2;
	game.player2.h = player2Height.value * game.tile;
	game.player2.y = middle - game.player2.h / 2;
	if (game.player2.y < 0) game.player2.y = 0;
	if (game.player2.y + game.player2.h > game.window.h)
	{
		game.player2.y = game.window.h - game.player2.h;
	}
	game.update_collide_full();
}

void Game::Setting::update_player1KeyUp(Game& game)
{
	game.player1.keyUp = player1KeyUp.value;
}

void Game::Setting::update_player1KeyDown(Game& game)
{
	game.player1.keyDown = player1KeyDown.value;
}

void Game::Setting::update_player2KeyUp(Game& game)
{
	game.player2.keyUp = player2KeyUp.value;
}

void Game::Setting::update_player2KeyDown(Game& game)
{
	game.player2.keyDown = player2KeyDown.value;
}

void Game::Setting::update_fps(Game& game) {
	game.timer.start(1000 / fps.value);
}

void Game::Setting::update_theme(Game& game)
{
	game.colorBack  = game.theme[theme.value].back;
	game.colorFront = game.theme[theme.value].front;
	game.ball.color = game.theme[theme.value].ball;
	game.player1.color = game.theme[theme.value].player1;
	game.player2.color = game.theme[theme.value].player2;
	game.scoreLeft  = game.font2(KTool::itoa(game.player1.score), game.colorFront);
	game.scoreRight = game.font2(KTool::itoa(game.player2.score), game.colorFront);
	game.menu.generate(game);
	return;
}
