#include <string>
#include <iostream>
#include "KTerm.hpp"
#include "KLib_TTF.hpp"

namespace KLib_TTF {
	bool active = false;
	void verbose(const std::string& message) {
		std::cout << KTerm::Bold << "[" << KTerm::Color(117) << "KLib_TTF"
		<< KTerm::UColor << "]" << KTerm::UBold << " " << KTerm::Color(117) << message
		<< KTerm::Def << std::endl;
	}
	void init() {
		if (not active) {
			if (TTF_Init() != 0) {
				verbose("enable to initialize");
				exit(-1);
			}
			active = true;
			verbose("init");
		}
	}
	void quit() {
		if (active) {
			TTF_Quit();
			active = false;
			verbose("quit");
		}
	}
}
