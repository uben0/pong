#include "klibrary/KLibrary.hpp"
#include <random>
#include <ctime>

typedef uint8_t   nat1;
typedef uint16_t  nat2;
typedef uint32_t  nat4;
typedef uint64_t  nat8;
typedef uintmax_t nat0;

typedef int8_t   int1;
typedef int16_t  int2;
typedef int32_t  int4;
typedef int64_t  int8;
typedef intmax_t int0;


class Game {
public:
	enum State {
		StateQuit,
		StatePlay,
		StateMenu,
		StateCatchKey,
		StateCreateServer,
		StateListServer,
		StateInfoServer,
		StatePlayServer,
		StateQuitServer
	} state;
	bool pause;
	KKey::Id catchedKey;
	KWindow window;
	KControl control;
	KTimer timer;

	struct Server {
		KSocketUdp socket;
		KSocket::Addr peerAddr;
		time_t lastUpdateTime;
		bool isClient;
		void get_peer_packages();
		void apply_state_to_game(Game& game);
		void fill_state_from_game(Game& game);
		void send_package_to_peer();
		struct Config {
			uint16_t width;
			uint16_t height;
			uint16_t player1Height;
			uint16_t player2Height;
			uint16_t player1Speed;
			uint16_t player2Speed;
			uint16_t ballSize;
			uint16_t ballSpeed;
			char name[8];
		} config;
		struct State {
			enum Graph {Handle, Passive} graph;
			enum Mov {
				Static = 0,
				Up     = 1,
				Down   = 2
			};
			bool play;
			bool quit;
			uint16_t playerPos;
			uint8_t  playerMov;
			uint16_t ballRad;
			uint16_t ballPosX;
			uint16_t ballPosY;
		} state;
		struct Package {
			enum Element {
				Point     = 0b00000001,
				PlayerPos = 0b00000010,
				BallState = 0b00000100,
				Handle    = 0b00001000,
				Pause     = 0b00010000,
				Play      = 0b00100000,
				Quit      = 0b01000000,
				PlayerMov = 0b10000000
			};
			uint8_t flag;
		} package;
		struct AvailableServer {
			std::time_t updateTime;
			KSocket::Addr address;
			Config config;
		};
		std::vector<AvailableServer> availableServers;
		struct MenuCreate : KRect<> {
			enum {
				BackToMenu,
				ServerName,
				Width,
				Height,
				Player1Height,
				Player2Height,
				Player1Speed,
				Player2Speed,
				BallSize,
				BallSpeed
			};
			unsigned line;
			KText title;
			KText label[10][2][2];
			void generate(Game& game);
			void print(KWindow& window, KColor cb, KColor cf, int tile);
		} menuCreate;
		struct MenuJoin : KRect<> {
			unsigned line;
			KText title;
			KText label[11][3][2];
			void generate(Game& game);
			void generate_server_list(Game& game);
			void print(KWindow& window, KColor cb, KColor cf, int tile);
			void line_up();
			void line_down();
			void change_enter(Game& game);
		} menuJoin;
		struct MenuInfo : KRect<> {
			enum {
				BackToMenu,
				Join,
				ServerName,
				Width,
				Height,
				Player1Height,
				Player2Height,
				Player1Speed,
				Player2Speed,
				BallSize,
				BallSpeed
			};
			unsigned line;
			KText title;
			KText label[11][2][2];
			void generate(Game& game);
			void print(KWindow& window, KColor cb, KColor cf, int tile);
			void line_up();
			void line_down();
			void change_enter(Game& game);
		} menuInfo;
	} server;

	void state_switch_menu_to_catchKey();
	void state_switch_catchKey_to_menu();

	void state_switch_menu_to_serverList();
	void state_switch_menu_to_serverCreate();
	void state_switch_serverList_to_serverInfo();
	void state_switch_serverInfo_to_serverList();
	void state_switch_serverInfo_to_playServer();
	void state_switch_serverCreate_to_playServer();
	void state_switch_serverList_to_menu();
	void state_switch_serverCreate_to_menu();
	void state_switch_playServer_to_serverQuit();
	void state_switch_playServer_to_menu();
	void state_switch_serverQuit_to_playServer();
	void state_switch_serverQuit_to_menu();

	void state_switch_menu_to_quit();
	void state_switch_serverCreate_to_quit();
	void state_switch_serverList_to_quit();
	void state_switch_serverInfo_to_quit();
	void state_switch_playServer_to_quit();
	void state_switch_serverQuit_to_quit();
	void state_switch_catchKey_to_quit();

	void state_update_serverCreate();
	void state_update_serverList();

	struct Theme {
		KColor back;
		KColor player1;
		KColor player2;
		KColor front;
		KColor ball;
		KColor midLine;
	} theme[5];
	KColor colorBack, colorFront;

	struct Menu : KRect<> {
		unsigned page, line;
		KText title;
		enum {
			PageNum        = 0,
			Player1KeyUp   = 1,
			Player1KeyDown = 2,
			Player2KeyUp   = 3,
			Player2KeyDown = 4,
			Player1Ai      = 5,
			Player2Ai      = 6,
			Player1Speed   = 7,
			Player2Speed   = 8,
			BallSpeed      = 9,
			ColorTheme     = 1,
			TileSize       = 2,
			Fps            = 3,
			Width          = 4,
			Height         = 5,
			Player1Height  = 6,
			Player2Height  = 7,
			Reset          = 1,
			CreateServer   = 2,
			JoinServer     = 3
		};
		KText label[3][10][2][2];
		void generate(Game& game);
		void print(KWindow& window, KColor cb, KColor cf, int tile);
		void line_up();
		void line_down();
		void change_left(Game& game);
		void change_right(Game& game);
		void change_enter(Game& game);
	} menu;

	struct Setting {
		void reset_all(Game& game);
		KSetting<int> tile;
		void update_tile(Game& game);
		KSetting<int> fps;
		void update_fps(Game& game);
		KSetting<int> width;
		void update_width(Game& game);
		KSetting<int> height;
		void update_height(Game& game);
		KSetting<int> ballSpeed;
		void update_ballSpeed(Game& game);
		KSetting<int> ballSize;
		void update_ballSize(Game& game);
		KSetting<bool> player1Ai;
		void update_player1Ai(Game& game);
		KSetting<bool> player2Ai;
		void update_player2Ai(Game& game);
		KSetting<int> player1Speed;
		void update_player1Speed(Game& game);
		KSetting<int> player2Speed;
		void update_player2Speed(Game& game);
		KSetting<int> player1Height;
		void update_player1Height(Game& game);
		KSetting<int> player2Height;
		void update_player2Height(Game& game);
		KSetting<KKey::Id> player1KeyUp;
		void update_player1KeyUp(Game& game);
		KSetting<KKey::Id> player1KeyDown;
		void update_player1KeyDown(Game& game);
		KSetting<KKey::Id> player2KeyUp;
		void update_player2KeyUp(Game& game);
		KSetting<KKey::Id> player2KeyDown;
		void update_player2KeyDown(Game& game);
		KSetting<unsigned> theme;
		void update_theme(Game& game);
		KSetting<KColor> colorBack;
		KSetting<KColor> colorPlayer1;
		KSetting<KColor> colorPlayer2;
		KSetting<KColor> colorFront;
		KSetting<KColor> colorBall;
		KSetting<KColor> colorMidLine;
		KSetting<std::string> serverName;
		KSetting<KSocket::Addr> membershipAddr;
		KSetting<bool> directConnect;
	} setting;

	int tile, width, height;
	struct Player : KRect<float, int> {
		KColor color;
		float speed;
		KKey::Id keyUp, keyDown;
		int score;
		bool artificialIntelligence;
	} player1, player2;
	struct Ball : KRect<float, int> {
		KColor color;
		double speed;
		double angle;
		float vx, vy;
		static constexpr double pi = 3.141592653589793238462643383279502884;
		void reset(float x, float y, const char* direction);
		void update_vector();
		void bounce(const char* direction);
		void deflect(Player& player);
	} ball;
	enum {
		CollideLeft,
		CollideRight,
		CollideTop,
		CollideBottom,
		CollidePlayer1Front,
		CollidePlayer2Front,
		CollidePlayer1Top,
		CollidePlayer2Top,
		CollidePlayer1Bottom,
		CollidePlayer2Bottom,
		CollidesAmount
	};
	KCollide::Border<int, int> collide[CollidesAmount];
	KFont font1, font2;
	KSurface scoreLeft, scoreRight;
	void update_collide_full();

	Game();
	~Game();
	void player_update_movement(KControl& control, Player& player, int time);
	void ball_update_movement();
	void draw_background();
	void draw_game();
	void draw_menu();
	void draw_menu_server_create();
	void draw_menu_server_list();
	void draw_menu_server_info();
	void draw_menu_server_quit();
	void update_display();
	void reset_positions(const char* direction);
};
