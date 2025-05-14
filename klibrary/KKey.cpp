#include "KKey.hpp"
#include "KTool.hpp"

const char* KKey::to_c_str(KKey::Id keyId) {
	switch (keyId) {
		case KKey::Digit0: return "0";
		case KKey::Digit1: return "1";
		case KKey::Digit2: return "2";
		case KKey::Digit3: return "3";
		case KKey::Digit4: return "4";
		case KKey::Digit5: return "5";
		case KKey::Digit6: return "6";
		case KKey::Digit7: return "7";
		case KKey::Digit8: return "8";
		case KKey::Digit9: return "9";
		case KKey::A: return "A";
		case KKey::B: return "B";
		case KKey::C: return "C";
		case KKey::D: return "D";
		case KKey::E: return "E";
		case KKey::F: return "F";
		case KKey::G: return "G";
		case KKey::H: return "H";
		case KKey::I: return "I";
		case KKey::J: return "J";
		case KKey::K: return "K";
		case KKey::L: return "L";
		case KKey::M: return "M";
		case KKey::N: return "N";
		case KKey::O: return "O";
		case KKey::P: return "P";
		case KKey::Q: return "Q";
		case KKey::R: return "R";
		case KKey::S: return "S";
		case KKey::T: return "T";
		case KKey::U: return "U";
		case KKey::V: return "V";
		case KKey::W: return "W";
		case KKey::X: return "X";
		case KKey::Y: return "Y";
		case KKey::Z: return "Z";
		case KKey::Backspace: return "Backspace";
		case KKey::Return:    return "Return";
		case KKey::Left:      return "Left";
		case KKey::Right:     return "Right";
		case KKey::Up:        return "Up";
		case KKey::Down:      return "Down";
		case KKey::LShift:    return "LShift";
		case KKey::RShift:    return "RShift";
		case KKey::LCtrl:     return "LCtrl";
		case KKey::RCtrl:     return "RCtrl";
		case KKey::LAlt:      return "LAlt";
		case KKey::RAlt:      return "RAlt";
		case KKey::Space:     return "Space";
		case KKey::Tab:       return "Tab";
		case KKey::Escape:    return "Escape";
		case KKey::None:      return "None";
		case KKey::LBracket:  return "LBracket";
		case KKey::RBracket:  return "RBracket";
		case KKey::LMeta:     return "LMeta";
		case KKey::RMeta:     return "RMeta";
		case KKey::F1:  return "F1";
		case KKey::F2:  return "F2";
		case KKey::F3:  return "F3";
		case KKey::F4:  return "F4";
		case KKey::F5:  return "F5";
		case KKey::F6:  return "F6";
		case KKey::F7:  return "F7";
		case KKey::F8:  return "F8";
		case KKey::F9:  return "F9";
		case KKey::F10: return "F10";
		case KKey::F11: return "F11";
		case KKey::F12: return "F12";
		case KKey::F13: return "F13";
		case KKey::F14: return "F14";
		case KKey::F15: return "F15";
		case KKey::F16: return "F16";
		case KKey::F17: return "F17";
		case KKey::F18: return "F18";
		case KKey::F19: return "F19";
		case KKey::F20: return "F20";
		case KKey::F21: return "F21";
		case KKey::F22: return "F22";
		case KKey::F23: return "F23";
		case KKey::F24: return "F24";
}
	static char buff[64];
	buff[0] = '<';
	KTool::itoa(int(keyId), buff + 1, 62);
	int i = 0;
	while (buff[i] != '\0') i++;
	buff[i] = '>';
	buff[i+1] = '\0';
	return buff;
}

KKey::Id KKey::from_c_str(const char* str) {
	if (str[0] == '\0') return KKey::None;
	if (str[1] == '\0') {
		switch (str[0]) {
			case '0': return KKey::Digit0;
			case '1': return KKey::Digit1;
			case '2': return KKey::Digit2;
			case '3': return KKey::Digit3;
			case '4': return KKey::Digit4;
			case '5': return KKey::Digit5;
			case '6': return KKey::Digit6;
			case '7': return KKey::Digit7;
			case '8': return KKey::Digit8;
			case '9': return KKey::Digit9;
			case 'A': return KKey::A;
			case 'B': return KKey::B;
			case 'C': return KKey::C;
			case 'D': return KKey::D;
			case 'E': return KKey::E;
			case 'F': return KKey::F;
			case 'G': return KKey::G;
			case 'H': return KKey::H;
			case 'I': return KKey::I;
			case 'J': return KKey::J;
			case 'K': return KKey::K;
			case 'L': return KKey::L;
			case 'M': return KKey::M;
			case 'N': return KKey::N;
			case 'O': return KKey::O;
			case 'P': return KKey::P;
			case 'Q': return KKey::Q;
			case 'R': return KKey::R;
			case 'S': return KKey::S;
			case 'T': return KKey::T;
			case 'U': return KKey::U;
			case 'V': return KKey::V;
			case 'W': return KKey::W;
			case 'X': return KKey::X;
			case 'Y': return KKey::Y;
			case 'Z': return KKey::Z;
		}
		return KKey::None;
	}
	if (str[0] == 'L') {
		switch (str[1]) {
			case 'e': return KKey::Left;
			case 'S': return KKey::LShift;
			case 'C': return KKey::LCtrl;
			case 'A': return KKey::LAlt;
			case 'B': return KKey::LBracket;
			case 'M': return KKey::LMeta;
		}
		return KKey::None;
	}
	if (str[0] == 'R') {
		switch (str[1]) {
			case 'i': return KKey::Right;
			case 'S': return KKey::RShift;
			case 'C': return KKey::RCtrl;
			case 'A': return KKey::RAlt;
			case 'B': return KKey::RBracket;
			case 'M': return KKey::RMeta;
		}
		return KKey::None;
	}
	if (str[0] == 'F' and str[1] != '\0') {
		if (str[2] == '\0') {
			switch (str[1]) {
				case '1': return KKey::F1;
				case '2': return KKey::F2;
				case '3': return KKey::F3;
				case '4': return KKey::F4;
				case '5': return KKey::F5;
				case '6': return KKey::F6;
				case '7': return KKey::F7;
				case '8': return KKey::F8;
				case '9': return KKey::F9;
			}
			return KKey::None;
		}
		else {
			if (str[1] == '1') {
				switch (str[2]) {
					case '0': return KKey::F10;
					case '1': return KKey::F11;
					case '2': return KKey::F12;
					case '3': return KKey::F13;
					case '4': return KKey::F14;
					case '5': return KKey::F15;
					case '6': return KKey::F16;
					case '7': return KKey::F17;
					case '8': return KKey::F18;
					case '9': return KKey::F19;
				}
				return KKey::None;
			}
			else if (str[1] == '2') {
				switch (str[2]) {
					case '0': return KKey::F20;
					case '1': return KKey::F21;
					case '2': return KKey::F22;
					case '3': return KKey::F23;
					case '4': return KKey::F24;
				}
				return KKey::None;
			}
			return KKey::None;
		}
	}
	if (str[0] == '<') {
		return KKey::Id(atoi(str+1));
	}
	switch(str[0]) {
		case 'U': return KKey::Up;
		case 'D': return KKey::Down;
		case 'S': return KKey::Space;
		case 'E': return KKey::Escape;
		case 'T': return KKey::Tab;
		case 'B': return KKey::Backspace;
	}
	return KKey::None;
}

std::ostream& operator<<(std::ostream& ostream, const KKey::Id& key) {
	const char* str = KKey::to_c_str(key);
	return ostream << str;
}

std::istream& operator>>(std::istream& istream, KKey::Id& key) {
	std::string buffer;
	istream >> buffer;
	key = KKey::from_c_str(buffer.c_str());
	return istream;
}
