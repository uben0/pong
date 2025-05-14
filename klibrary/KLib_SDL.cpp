#include <string>
#include <iostream>
#include <cassert>
#include "KWindow.hpp"
#include "KTerm.hpp"
#include "KColor.hpp"
#include "KLib_SDL.hpp"

namespace KLib_SDL {
	SDL_Surface*    surface = nullptr;
	SDL_PixelFormat* format = nullptr;
	Linker*          linker = nullptr;
	void verbose(const std::string& message) {
		std::cout << KTerm::Bold << "[" << KTerm::Color(117) << "KLib_SDL"
		<< KTerm::UColor << "]" << KTerm::UBold << " " << KTerm::Color(117) << message
		<< KTerm::Def << std::endl;
	}
	void initialize() {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			verbose("unable to initialize");
			exit(-1);
		}
		linker = new Linker();
		surface = SDL_CreateRGBSurface(
			0, 1, 1, 32,
			KColor{255, 0, 0, 0},
			KColor{0, 255, 0, 0},
			KColor{0, 0, 255, 0},
			KColor{0, 0, 0, 255}
		);
		format = surface->format;
		verbose("init");
	}
	void uninitialize() {
		SDL_FreeSurface(surface);
		SDL_Quit();
		delete linker;
		verbose("quit");
	}

	Linker::Linker(): m_firstFree(0), m_ids(), m_windows() {}
	unsigned int Linker::new_id() {
		if (m_ids.size() <= m_firstFree) {
			m_ids.push_back(true);
			m_firstFree = m_ids.size();
			assert(m_firstFree < (unsigned int)-1);
			return m_firstFree - 1;
		}
		else {
			m_ids[m_firstFree] = true;
			unsigned int newId = m_firstFree;
			while (m_firstFree < m_ids.size() && m_ids[m_firstFree]) m_firstFree++;
			return newId;
		}
	}
	void Linker::del_id(unsigned int id) {
		assert(id < m_ids.size());
		for (unsigned int i = 0; i < m_windows.size(); i++) {
			m_windows[i]->remove_texture(id);
		}
		if (id == m_ids.size() - 1) {
			m_ids.pop_back();
		}
		else {
			m_ids[id] = false;
			m_firstFree = std::min(id, m_firstFree);
		}
	}
	void Linker::add_window(KWindow* window) {
		m_windows.push_back(window);
	}
	void Linker::remove_window(KWindow* window) {
		for (unsigned int i = 0; i < m_windows.size(); i++) {
			if (m_windows[i] == window) {
				m_windows.erase(m_windows.begin() + i);
				break;
			}
		}
	}
	void Linker::update_events() {
		SDL_Event event;
		if (m_windows.size() == 0) {
			while (SDL_PollEvent(&event));
			return;
		}

		KWindow* window = m_windows[0];
		while (SDL_PollEvent(&event)) {
			if (
				event.type == SDL_KEYDOWN or
				event.type == SDL_WINDOWEVENT or
				event.type == SDL_MOUSEBUTTONDOWN
			) {
				if (window->m_id != event.window.windowID) {
					window = (KWindow*)SDL_GetWindowData(
						SDL_GetWindowFromID(event.window.windowID), "this"
					);
					if (window == nullptr) {
						window = m_windows[0];
					}
				}
				if (window->m_id == event.window.windowID) {
					switch(event.type) {
						case SDL_KEYDOWN:
						window->event.m_key.insert((KKey::Id)event.key.keysym.sym);
						break;
						case SDL_WINDOWEVENT:
						if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
							window->event.m_close = true;
						}
						else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
							window->update_size();
						}
						break;
						case SDL_MOUSEBUTTONDOWN:
						window->event.m_mouse.x = event.button.x;
						window->event.m_mouse.y = event.button.y;
						switch (event.button.button) {
							case SDL_BUTTON_LEFT:
							window->event.m_mouse.l = true;
							break;
							case SDL_BUTTON_MIDDLE:
							window->event.m_mouse.m = true;
							break;
							case SDL_BUTTON_RIGHT:
							window->event.m_mouse.r = true;
							break;
						}
					}
				}
			}
			else if (event.type == SDL_QUIT) {
				for (unsigned int i = 0; i < m_windows.size(); i++) {
					m_windows[i]->event.m_quit = true;
				}
			}
		}
	}
}
