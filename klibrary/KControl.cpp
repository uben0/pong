#include <cassert>
#include <SDL2/SDL.h>

#include "KControl.hpp"

KControl::KControl() : key(SDL_GetKeyboardState(nullptr)), mouse{0, 0, 0, 0, 0} {
	assert(key != nullptr);
}

void KControl::update() {
	SDL_PumpEvents();
	unsigned long state = SDL_GetMouseState(&mouse.x, &mouse.y);
	mouse.l = state & SDL_BUTTON(SDL_BUTTON_LEFT);
	mouse.m = state & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	mouse.r = state & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

bool KControl::operator[](KKey::Id keyId) const {
	if (keyId == KKey::None) return false;
	return key[SDL_GetScancodeFromKey(keyId)];
}
