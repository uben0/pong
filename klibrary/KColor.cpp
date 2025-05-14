#include "KColor.hpp"

const KColor KColor::White        = 0xffffffff;
const KColor KColor::Black        = 0x000000ff;
const KColor KColor::Red          = 0xff0000ff;
const KColor KColor::Green        = 0x00ff00ff;
const KColor KColor::Blue         = 0x0000ffff;
const KColor KColor::Cyan         = 0x00ffffff;
const KColor KColor::Magenta      = 0xff00ffff;
const KColor KColor::Yellow       = 0xffff00ff;
const KColor KColor::Transparent  = 0x00000000;

KColor::KColor() : r(0), g(0), b(0), a(0) {}

KColor::KColor(unsigned long rgba) {
	a = rgba & 0xff;
	rgba = rgba >> 8;
	b = rgba & 0xff;
	rgba = rgba >> 8;
	g = rgba & 0xff;
	rgba = rgba >> 8;
	r = rgba & 0xff;
}

KColor::KColor(
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a
) : r(r), g(g), b(b), a(a) {}

SDL_Color KColor::to_SDL_Color() const {
	return SDL_Color{r, g, b, a};
}

std::ostream& operator<<(std::ostream& ostream, const KColor& color) {
	ostream << int(color.r) << " ";
	ostream << int(color.g) << " ";
	ostream << int(color.b);
	if (color.a != 255)
	{
		ostream << " " << int(color.a);
	}
	return ostream;
}

std::istream& operator>>(std::istream& istream, KColor& color) {
	int r = 0, g = 0, b = 0, a = 255;
	istream >> r;
	istream >> g;
	istream >> b;
	istream >> a;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return istream;
}

KColor& KColor::operator+=(KColor added) {
	r = std::min(((int)r + added.r), 255);
	g = std::min(((int)g + added.g), 255);
	b = std::min(((int)b + added.b), 255);
	a = std::min(((int)a + added.a), 255);
	return *this;
}
