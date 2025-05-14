#include "Game.hpp"

void Game::Menu::change_left(Game& game)
{
	if (line == PageNum)
	{
		if (page > 0) page--;
	}
	else if (page == 0)
	{
		if (line == Player1Speed)
		{
			game.setting.player1Speed.value--;
			game.setting.update_player1Speed(game);
			label[page][line][1][0].change(std::to_string(game.setting.player1Speed.value));
			label[page][line][1][1].change(std::to_string(game.setting.player1Speed.value));
		}
		else if (line == Player2Speed)
		{
			game.setting.player2Speed.value--;
			game.setting.update_player2Speed(game);
			label[page][line][1][0].change(std::to_string(game.setting.player2Speed.value));
			label[page][line][1][1].change(std::to_string(game.setting.player2Speed.value));
		}
		else if (line == BallSpeed)
		{
			game.setting.ballSpeed.value--;
			game.setting.update_ballSpeed(game);
			label[page][line][1][0].change(std::to_string(game.setting.ballSpeed.value));
			label[page][line][1][1].change(std::to_string(game.setting.ballSpeed.value));
		}
	}
	else if (page == 1)
	{
		if (line == ColorTheme)
		{
			if (game.setting.theme.value > 0)
			{
				game.setting.theme.value--;
				game.setting.update_theme(game);
				label[page][line][1][0].change(std::to_string(game.setting.theme.value));
				label[page][line][1][1].change(std::to_string(game.setting.theme.value));
			}
		}
		else if (line == TileSize)
		{
			if (game.setting.tile.value > 1)
			{
				game.setting.tile.value--;
				game.setting.update_tile(game);
				label[page][line][1][0].change(std::to_string(game.setting.tile.value));
				label[page][line][1][1].change(std::to_string(game.setting.tile.value));
			}
		}
		else if (line == Fps)
		{
			if (game.setting.fps.value > 1)
			{
				game.setting.fps.value--;
				game.setting.update_fps(game);
				label[page][line][1][0].change(std::to_string(game.setting.fps.value));
				label[page][line][1][1].change(std::to_string(game.setting.fps.value));
			}
		}
		else if (line == Width)
		{
			if (game.setting.width.value > 1)
			{
				game.setting.width.value--;
				game.setting.update_width(game);
				label[page][line][1][0].change(std::to_string(game.setting.width.value));
				label[page][line][1][1].change(std::to_string(game.setting.width.value));
			}
		}
		else if (line == Height)
		{
			if (game.setting.height.value > 1)
			{
				game.setting.height.value--;
				game.setting.update_height(game);
				label[page][line][1][0].change(std::to_string(game.setting.height.value));
				label[page][line][1][1].change(std::to_string(game.setting.height.value));
			}
		}
		else if (line == Player1Height)
		{
			if (game.setting.player1Height.value > 1)
			{
				game.setting.player1Height.value--;
				game.setting.update_player1Height(game);
				label[page][line][1][0].change(std::to_string(game.setting.player1Height.value));
				label[page][line][1][1].change(std::to_string(game.setting.player1Height.value));
			}
		}
		else if (line == Player2Height)
		{
			if (game.setting.player2Height.value > 1)
			{
				game.setting.player2Height.value--;
				game.setting.update_player2Height(game);
				label[page][line][1][0].change(std::to_string(game.setting.player2Height.value));
				label[page][line][1][1].change(std::to_string(game.setting.player2Height.value));
			}
		}
	}
}

void Game::Menu::change_right(Game& game)
{
	if (line == PageNum)
	{
		if (page < 2) page++;
	}
	else if (page == 0)
	{
		if (line == Player1Speed)
		{
			game.setting.player1Speed.value++;
			game.setting.update_player1Speed(game);
			label[page][line][1][0].change(std::to_string(game.setting.player1Speed.value));
			label[page][line][1][1].change(std::to_string(game.setting.player1Speed.value));
		}
		else if (line == Player2Speed)
		{
			game.setting.player2Speed.value++;
			game.setting.update_player2Speed(game);
			label[page][line][1][0].change(std::to_string(game.setting.player2Speed.value));
			label[page][line][1][1].change(std::to_string(game.setting.player2Speed.value));
		}
		else if (line == BallSpeed)
		{
			game.setting.ballSpeed.value++;
			game.setting.update_ballSpeed(game);
			label[page][line][1][0].change(std::to_string(game.setting.ballSpeed.value));
			label[page][line][1][1].change(std::to_string(game.setting.ballSpeed.value));
		}
	}
	else if (page == 1)
	{
		if (line == ColorTheme)
		{
			if (game.setting.theme.value < KTool::lenof(game.theme) - 1)
			{
				game.setting.theme.value++;
				game.setting.update_theme(game);
				label[page][line][1][0].change(std::to_string(game.setting.theme.value));
				label[page][line][1][1].change(std::to_string(game.setting.theme.value));
			}
		}
		else if (line == TileSize)
		{
			game.setting.tile.value++;
			game.setting.update_tile(game);
			label[page][line][1][0].change(std::to_string(game.setting.tile.value));
			label[page][line][1][1].change(std::to_string(game.setting.tile.value));
		}
		else if (line == Fps)
		{
			game.setting.fps.value++;
			game.setting.update_fps(game);
			label[page][line][1][0].change(std::to_string(game.setting.fps.value));
			label[page][line][1][1].change(std::to_string(game.setting.fps.value));
		}
		else if (line == Width)
		{
			game.setting.width.value++;
			game.setting.update_width(game);
			label[page][line][1][0].change(std::to_string(game.setting.width.value));
			label[page][line][1][1].change(std::to_string(game.setting.width.value));
		}
		else if (line == Height)
		{
			game.setting.height.value++;
			game.setting.update_height(game);
			label[page][line][1][0].change(std::to_string(game.setting.height.value));
			label[page][line][1][1].change(std::to_string(game.setting.height.value));
		}
		else if (line == Player1Height)
		{
			game.setting.player1Height.value++;
			game.setting.update_player1Height(game);
			label[page][line][1][0].change(std::to_string(game.setting.player1Height.value));
			label[page][line][1][1].change(std::to_string(game.setting.player1Height.value));
		}
		else if (line == Player2Height)
		{
			game.setting.player2Height.value++;
			game.setting.update_player2Height(game);
			label[page][line][1][0].change(std::to_string(game.setting.player2Height.value));
			label[page][line][1][1].change(std::to_string(game.setting.player2Height.value));
		}
	}
}

void Game::Menu::change_enter(Game& game)
{
	if (page == 0)
	{
		if (line == Player1Ai)
		{
			game.setting.player1Ai.value = not game.setting.player1Ai.value;
			game.setting.update_player1Ai(game);
			label[page][line][1][0].change(game.setting.player1Ai.value ? "true" : "false");
			label[page][line][1][1].change(game.setting.player1Ai.value ? "true" : "false");
		}
		else if (line == Player2Ai)
		{
			game.setting.player2Ai.value = not game.setting.player2Ai.value;
			game.setting.update_player2Ai(game);
			label[page][line][1][0].change(game.setting.player2Ai.value ? "true" : "false");
			label[page][line][1][1].change(game.setting.player2Ai.value ? "true" : "false");
		}
		else if (line == Player1KeyUp)
		{
			game.state_switch_menu_to_catchKey();
		}
		else if (line == Player1KeyDown)
		{
			game.state_switch_menu_to_catchKey();
		}
		else if (line == Player2KeyUp)
		{
			game.state_switch_menu_to_catchKey();
		}
		else if (line == Player2KeyDown)
		{
			game.state_switch_menu_to_catchKey();
		}
	}
	else if (page == 2) {
		if (line == Reset) {
			game.setting.reset_all(game);
			page = 0;
			line = 0;
		}
		else if (line == CreateServer) {
			game.state_switch_menu_to_serverCreate();
		}
		else if (line == JoinServer) {
			game.state_switch_menu_to_serverList();
		}
	}
}

void Game::Server::MenuJoin::line_up() {
	if (line > 0) line -= 1;
}

void Game::Server::MenuJoin::line_down() {
	if (line + 1 < KTool::lenof(label)) line += 1;
}

void Game::Server::MenuInfo::line_up() {
	if (line == Join) line = BackToMenu;
}

void Game::Server::MenuInfo::line_down() {
	if (line == BackToMenu) line = Join;
}

void Game::Server::MenuJoin::change_enter(Game& game) {
	if (line == 0) {
		game.state_switch_serverList_to_menu();
	}
	else if (line < KTool::lenof(label) && line - 1 < game.server.availableServers.size()) {
		game.state_switch_serverList_to_serverInfo();
	}
}

void Game::Server::MenuInfo::change_enter(Game& game) {
	if (line == BackToMenu) {
		game.state_switch_serverInfo_to_serverList();
	}
	else if (line == Join) {
		game.state_switch_serverInfo_to_playServer();
	}
}
