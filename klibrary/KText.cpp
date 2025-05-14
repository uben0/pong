#include "KText.hpp"

KText::KText():
KSurface(),
m_str(),
m_font(),
m_color()
{}

KText::KText(std::string const& str, const KFont& font, KColor color):
KSurface(),
m_str(str),
m_font(font),
m_color(color)
{
	update();
}

KText::KText(const char* str, const KFont& font, KColor color):
KSurface(),
m_str(str),
m_font(font),
m_color(color)
{
	update();
}

void KText::assign(std::string const& str, const KFont& font, KColor color) {
	m_str = str;
	m_font = font;
	m_color = color;
	update();
}

void KText::assign(const char* str, const KFont& font, KColor color) {
	m_str = str;
	m_font = font;
	m_color = color;
	update();
}

void KText::set(std::string const& str) {
	m_str = str;
}

void KText::set(const char* str) {
	m_str = str;
}

void KText::set(KFont& font) {
	m_font = font;
}

void KText::set(KColor color) {
	m_color = color;
}

void KText::update() {
	KSurface::operator=(m_font(m_str, m_color));
}

void KText::change(std::string const& str) {
	m_str = str;
	update();
}

void KText::change(const char* str) {
	m_str = str;
	update();
}

void KText::change(KFont& font) {
	m_font = font;
	update();
}

void KText::change(KColor color) {
	m_color = color;
	update();
}
