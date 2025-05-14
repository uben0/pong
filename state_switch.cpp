#include "Game.hpp"

void Game::state_switch_menu_to_catchKey() {
	state = StateCatchKey;
	menu.label[menu.page][menu.line][1][1].change("_");
}

void Game::state_switch_catchKey_to_menu() {
	if (menu.line == menu.Player1KeyUp)
	{
		setting.player1KeyUp.value = catchedKey;
		catchedKey = KKey::None;
		setting.update_player1KeyUp(*this);
		menu.label[0][menu.line][1][0].change(KKey::to_c_str(setting.player1KeyUp.value));
		menu.label[0][menu.line][1][1].change(KKey::to_c_str(setting.player1KeyUp.value));
	}
	else if (menu.line == menu.Player1KeyDown)
	{
		setting.player1KeyDown.value = catchedKey;
		catchedKey = KKey::None;
		setting.update_player1KeyDown(*this);
		menu.label[0][menu.line][1][0].change(KKey::to_c_str(setting.player1KeyDown.value));
		menu.label[0][menu.line][1][1].change(KKey::to_c_str(setting.player1KeyDown.value));
	}
	else if (menu.line == menu.Player2KeyUp)
	{
		setting.player2KeyUp.value = catchedKey;
		catchedKey = KKey::None;
		setting.update_player2KeyUp(*this);
		menu.label[0][menu.line][1][0].change(KKey::to_c_str(setting.player2KeyUp.value));
		menu.label[0][menu.line][1][1].change(KKey::to_c_str(setting.player2KeyUp.value));
	}
	else if (menu.line == menu.Player2KeyDown)
	{
		setting.player2KeyDown.value = catchedKey;
		catchedKey = KKey::None;
		setting.update_player2KeyDown(*this);
		menu.label[0][menu.line][1][0].change(KKey::to_c_str(setting.player2KeyDown.value));
		menu.label[0][menu.line][1][1].change(KKey::to_c_str(setting.player2KeyDown.value));
	}
	state = StateMenu;
}

void Game::state_switch_menu_to_serverList() {
	if (not server.socket.bind({"::", 0})) {
		std::ofstream logFile("log.txt", std::ios::app);
		logFile << KTimer::date() << " [ERROR] " << server.socket.error << std::endl;
		logFile.close();
		return;
	}
	state = StateListServer;
	server.lastUpdateTime = 0;
	server.menuJoin.line = 0;
	server.menuJoin.generate(*this);
}

void Game::state_switch_menu_to_serverCreate() {
	if (not server.socket.bind({setting.membershipAddr.value.is_ipv4() ? "0.0.0.0":"::", setting.membershipAddr.value.get_port()})) {
		std::ofstream logFile("log.txt", std::ios::app);
		logFile << KTimer::date() << " [ERROR] " << server.socket.error << std::endl;
		logFile.close();
		return;
	}
	if (not setting.directConnect.value) {
		if (not server.socket.join_membership(setting.membershipAddr.value)) {
			std::ofstream logFile("log.txt", std::ios::app);
			logFile << KTimer::date() << " [ERROR] " << server.socket.error << std::endl;
			logFile.close();
			return;
		}
	}

	state = StateCreateServer;
	// set server name
	if (setting.serverName.value.empty()) {
		srand(time(nullptr));
		assert(KTool::lenof(server.config.name) >= 5);
		setting.serverName.value.resize(5, ' ');
		setting.serverName.value[0] = (rand() % 8) + 'a';
		setting.serverName.value[1] = (rand() % 8) + 'a';
		setting.serverName.value[2] = '-';
		setting.serverName.value[3] = (rand() % 10) + '0';
		setting.serverName.value[4] = (rand() % 10) + '0';
	}
	if (setting.serverName.value.size() > KTool::lenof(server.config.name)) {
		setting.serverName.value.resize(KTool::lenof(server.config.name), ' ');
	}
	for (unsigned i = 0; i < KTool::lenof(server.config.name); i++) {
		server.config.name[i] = '\0';
	}
	for (unsigned i = 0; i < setting.serverName.value.size(); i++) {
		server.config.name[i] = setting.serverName.value[i];
	}
	server.lastUpdateTime = 0;
	server.menuCreate.line = 0;
	server.config.width = width;
	server.config.height = height;
	server.config.player1Height = setting.player1Height.value;
	server.config.player2Height = setting.player2Height.value;
	server.config.player1Speed = setting.player1Speed.value;
	server.config.player2Speed = setting.player2Speed.value;
	server.config.ballSize = setting.ballSize.value;
	server.config.ballSpeed = setting.ballSpeed.value;
	server.menuCreate.generate(*this);
}

void Game::state_switch_serverList_to_serverInfo() {
	server.config = server.availableServers[server.menuJoin.line - 1].config;
	server.peerAddr = server.availableServers[server.menuJoin.line - 1].address;
	server.menuInfo.generate(*this);
	state = StateInfoServer;
}

void Game::state_switch_serverInfo_to_serverList() {
	state = StateListServer;
}

void Game::state_switch_serverInfo_to_playServer() {
	state = StatePlayServer;
	server.state.graph = server.state.Passive;
	server.isClient = true;
	server.lastUpdateTime = std::time(nullptr);

	player1.artificialIntelligence = false;
	player2.artificialIntelligence = false;
	player1.h = server.config.player2Height * tile;
	player2.h = server.config.player1Height * tile;

	player1.speed = server.config.player2Speed / 1000.0 * tile;
	player2.speed = server.config.player1Speed / 1000.0 * tile;
	ball.w = server.config.ballSize * tile;
	ball.h = server.config.ballSize * tile;
	ball.speed = server.config.ballSpeed / 1000.0 * tile;
	ball.update_vector();

	width  = server.config.width;
	height = server.config.height;
	window.resize(width * tile, height * tile);
	player2.x = window.w - 2 * tile;
	reset_positions("left");
	update_collide_full();
	server.package.flag = 0;
	server.state.play = true;
}

void Game::state_switch_serverCreate_to_playServer() {
	state = StatePlayServer;
	server.socket.drop_membership(setting.membershipAddr.value);
	server.state.graph = server.state.Handle;
	server.isClient = false;
	server.lastUpdateTime = std::time(nullptr);

	player1.artificialIntelligence = false;
	player2.artificialIntelligence = false;
	player1.h = server.config.player1Height * tile;
	player2.h = server.config.player2Height * tile;

	player1.speed = server.config.player1Speed / 1000.0 * tile;
	player2.speed = server.config.player2Speed / 1000.0 * tile;
	ball.speed = server.config.ballSpeed / 1000.0 * tile;
	ball.update_vector();

	width  = server.config.width;
	height = server.config.height;
	window.resize(width * tile, height * tile);
	player2.x = window.w - 2 * tile;
	reset_positions("right");
	update_collide_full();
	server.package.flag = 0;
	server.state.play = true;
}

void Game::state_switch_serverList_to_menu() {
	server.availableServers.clear();
	server.socket.close();
	state = StateMenu;
}

void Game::state_switch_serverCreate_to_menu() {
	server.socket.drop_membership(setting.membershipAddr.value);
	server.socket.close();
	state = StateMenu;
}

void Game::state_switch_playServer_to_serverQuit() {
	state = StateQuitServer;
}

void Game::state_switch_playServer_to_menu() {
	state_switch_playServer_to_serverQuit();
	state_switch_serverQuit_to_menu();
}

void Game::state_switch_serverQuit_to_playServer() {
	state = StatePlayServer;
	server.lastUpdateTime = std::time(nullptr);
}

void Game::state_switch_serverQuit_to_menu() {
	uint16_t quitFlag = server.package.Quit;
	server.socket.write_var(server.peerAddr, quitFlag);
	server.socket.close();
	setting.update_width(*this);
	setting.update_height(*this);
	setting.update_ballSpeed(*this);
	setting.update_ballSize(*this);
	setting.update_player1Ai(*this);
	setting.update_player2Ai(*this);
	setting.update_player1Height(*this);
	setting.update_player2Height(*this);
	setting.update_player1Speed(*this);
	setting.update_player2Speed(*this);
	state = StateMenu;
}

void Game::state_switch_menu_to_quit() {
	state = StateQuit;
}

void Game::state_switch_serverCreate_to_quit() {
	state_switch_serverCreate_to_menu();
	state_switch_menu_to_quit();
}

void Game::state_switch_serverList_to_quit() {
	state_switch_serverList_to_menu();
	state_switch_menu_to_quit();
}

void Game::state_switch_serverInfo_to_quit() {
	state_switch_serverInfo_to_serverList();
	state_switch_serverList_to_quit();
}

void Game::state_switch_playServer_to_quit() {
	state_switch_playServer_to_serverQuit();
	state_switch_serverQuit_to_quit();
}

void Game::state_switch_serverQuit_to_quit() {
	state_switch_serverQuit_to_menu();
	state_switch_menu_to_quit();
}

void Game::state_switch_catchKey_to_quit() {
	state = StateQuit;
}
