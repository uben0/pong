#include "KEvent.hpp"
#include "KWindow.hpp"

KEvent::KEvent():
m_key(),
m_mouse{0, 0, false, false, false},
m_close(false),
m_quit(false),
key(),
mouse{0, 0, false, false, false},
close(false),
quit(false)
{}

void KEvent::update() {
	KLib_SDL::linker->update_events();

	// put back data to front
	key     = std::move(m_key);
	mouse.x = m_mouse.x;
	mouse.y = m_mouse.y;
	mouse.l = m_mouse.l;
	mouse.m = m_mouse.m;
	mouse.r = m_mouse.r;
	close   = m_close;
	quit    = m_quit;

	// clear back data
	m_mouse.x = 0;
	m_mouse.y = 0;
	m_mouse.l = false;
	m_mouse.m = false;
	m_mouse.r = false;
	m_close   = false;
	m_quit    = false;
}

KKey::Id KEvent::get_key() const {
	if (key.size() == 0) return KKey::None;
	return *key.begin();
}

KKey::Id KEvent::pop_key() {
	if (key.size() == 0) return KKey::None;
	auto iter = key.begin();
	KKey::Id result = *iter;
	key.erase(iter);
	return result;
}

void KEvent::add_key(KKey::Id keyId) {
	key.insert(keyId);
}

bool KEvent::operator[](KKey::Id keyId) const {
	return key.find(keyId) != key.end();
}
