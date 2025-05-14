#include "KTerm.hpp"

namespace KTerm {
	std::string Color(unsigned char colorId) {
		return "\e[38;5;" + std::to_string(int(colorId)) + "m";
	}
}
