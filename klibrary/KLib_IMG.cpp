#include <string>
#include <iostream>
#include "KTerm.hpp"
#include "KLib_IMG.hpp"

namespace KLib_IMG {
	bool active = false;
	void verbose(const std::string& message) {
		std::cout << KTerm::Bold << "[" << KTerm::Color(117) << "KLib_IMG"
		<< KTerm::UColor << "]" << KTerm::UBold << " " << KTerm::Color(117) << message
		<< KTerm::Def << std::endl;
	}
	void init() {
		if (not active) {
			if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) == 0) {
				verbose("enable to initialize");
				exit(-1);
			}
			active = true;
			verbose("init");
		}
	}
	void quit() {
		if (active) {
			IMG_Quit();
			active = false;
			verbose("quit");
		}
	}
}
