#include "Game.hpp"

int main(int, char const *[])
{
	bool loop = true;
	Game game;
	KEvent& event = game.window.event;
	KControl control;
	while (loop)
	{
		game.draw_background();
		switch (game.state) {
			case game.StateQuit: {
				loop = false;
			}
			break;
		case game.StatePlay: {
				if (not game.pause)
				{
					control.update();
					game.player_update_movement(control, game.player1, game.timer.ms);
					game.player_update_movement(control, game.player2, game.timer.ms);
					game.ball_update_movement();
					if (event[KKey::Escape])
					{
						game.pause = true;
					}
				}
				else
				{
					KKey::Id pressedKey = event.get_key();
					if (
						pressedKey != KKey::None and (
							pressedKey == game.player1.keyUp or
							pressedKey == game.player1.keyDown or
							pressedKey == game.player2.keyUp or
							pressedKey == game.player2.keyDown or
							pressedKey == KKey::Escape or
							pressedKey == KKey::Return or
							pressedKey == KKey::Space or
							pressedKey == KKey::Backspace
						)
					) game.pause = false;
				}
				game.draw_game();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state = game.StateQuit;
				}
				else if (event[KKey::M])
				{
					game.state = game.StateMenu;
				}
			}
			break;
			case game.StateMenu: {
				game.draw_game();
				game.draw_menu();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state = game.StateQuit;
				}
				else if (event[KKey::Escape] or event[KKey::M] or event[KKey::Backspace])
				{
					game.state = game.StatePlay;
				}
				else if (event[KKey::Up])
				{
					game.menu.line_up();
				}
				else if (event[KKey::Down])
				{
					game.menu.line_down();
				}
				else if (event[KKey::Left])
				{
					game.menu.change_left(game);
				}
				else if (event[KKey::Right])
				{
					game.menu.change_right(game);
				}
				else if (event[KKey::Return])
				{
					game.menu.change_enter(game);
				}
			}
			break;
			case game.StateCatchKey: {
				game.draw_game();
				game.draw_menu();
				game.catchedKey = event.get_key();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state_switch_catchKey_to_quit();
				}
				else if (game.catchedKey != KKey::None)
				{
					game.state_switch_catchKey_to_menu();
				}
			}
			break;
			case game.StateCreateServer: {
				game.state_update_serverCreate();
				game.draw_game();
				game.draw_menu_server_create();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state_switch_serverCreate_to_quit();
				}
				else if (event[KKey::Escape] or event[KKey::Backspace] or event[KKey::Return])
				{
					game.state_switch_serverCreate_to_menu();
				}
			}
			break;
			case game.StateListServer: {
				game.state_update_serverList();
				game.draw_game();
				game.draw_menu_server_list();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state_switch_serverList_to_quit();
				}
				else if (event[KKey::Escape] or event[KKey::Backspace])
				{
					game.state_switch_serverList_to_menu();
				}
				else if (event[KKey::Return]) {
					game.server.menuJoin.change_enter(game);
				}
				else if (event[KKey::Up])
				{
					game.server.menuJoin.line_up();
				}
				else if (event[KKey::Down])
				{
					game.server.menuJoin.line_down();
				}
			}
			break;
			case game.StateInfoServer: {
				game.draw_game();
				game.draw_menu_server_info();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state_switch_serverInfo_to_quit();
				}
				else if (event[KKey::Escape] or event[KKey::Backspace]) {
					game.state_switch_serverInfo_to_serverList();
				}
				else if (event[KKey::Return]) {
					game.server.menuInfo.change_enter(game);
				}
				else if (event[KKey::Up])
				{
					game.server.menuInfo.line_up();
				}
				else if (event[KKey::Down])
				{
					game.server.menuInfo.line_down();
				}
			}
			break;
			case game.StatePlayServer: {
				game.server.get_peer_packages();
				game.server.apply_state_to_game(game);
				control.update();
				game.player_update_movement(control, game.player2, game.timer.ms);
				if (game.server.state.graph == game.server.state.Handle) {
					int score = game.player1.score;
					game.ball_update_movement();
					game.server.package.flag = 0b00100110;
					if (score != game.player1.score) {
						game.server.package.flag |= 0b00000001;
					}
				}
				else {
					game.server.package.flag = 0b00100010;
				}
				game.draw_game();
				game.server.fill_state_from_game(game);
				game.server.send_package_to_peer();
				if (game.server.lastUpdateTime + 2 < std::time(nullptr)) {
					game.state_switch_playServer_to_menu();
				}
				if (event.quit or event.close or event[KKey::Q]) {
					game.state_switch_playServer_to_quit();
				}
				else if (event[KKey::Escape] or event[KKey::Backspace]) {
					game.state_switch_playServer_to_serverQuit();
				}
			}
			break;
			case game.StateQuitServer: {
				game.server.get_peer_packages();
				game.server.apply_state_to_game(game);
				game.draw_game();
				game.server.send_package_to_peer();
				if (event.quit or event.close or event[KKey::Q]) {
					game.state_switch_serverQuit_to_quit();
				}
				else if (event[KKey::Escape] or event[KKey::Backspace]) {
					game.state_switch_serverQuit_to_menu();
				}
				else if (event[KKey::Return]) {
					game.state_switch_serverQuit_to_playServer();
				}
			}
			break;
		}
		game.update_display();
		game.timer.wait();
		event.update();
	}
	return EXIT_SUCCESS;
}
