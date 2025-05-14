#include <cassert>
#include <iostream>
#include "KFont.hpp"

KFont::KFont() : m_shared(nullptr) {
}

KFont::KFont(const std::string& file, int size) : KFont() {
	open(file, size);
}

KFont::KFont(const KFont& font): KFont() {
	if (font.is_opened()) {
		m_shared = font.m_shared;
		m_shared->refCount += 1;
	}
}

KFont::~KFont() {
	close();
}

bool KFont::is_opened() const {
	return m_shared != nullptr;
}

bool KFont::is_closed() const {
	return m_shared == nullptr;
}

KFont& KFont::operator=(const KFont& font) {
	if (font.is_opened()) {
		font.m_shared->refCount += 1;
		close();
		m_shared = font.m_shared;
	}
	else {
		close();
	}
	return *this;
}

bool KFont::open(const std::string& file, int size) {
	close();
	KLib_TTF::init();
	m_shared = new Shared;
	m_shared->refCount = 1;
	m_shared->font = TTF_OpenFont(file.c_str(), size);
	if (m_shared->font == nullptr) {
		delete m_shared;
		m_shared = nullptr;
	}
	return is_opened();
}

void KFont::close() {
	if (m_shared != nullptr) {
		m_shared->refCount -= 1;
		if (m_shared->refCount == 0) {
			TTF_CloseFont(m_shared->font);
			delete m_shared;
		}
		m_shared = nullptr;
	}
}

KSurface KFont::operator()(const std::string& text, KColor color) {
	if (is_opened()) {
		KSurface surface(TTF_RenderText_Blended(m_shared->font, text.c_str(), color.to_SDL_Color()));
		KColor* map = surface.get_map();
		for (int i = 0; i < surface.w * surface.h; i++) {
			map[i].a = map[i].a * color.a / 255;
		}
		return surface;
	}
	return KSurface();
}
