#include "KSurface.hpp"
#include <cassert>

KSurface::KSurface() : KRect(), m_shared(nullptr) {
}

KSurface::KSurface(SDL_Surface* src) : KSurface() {
	from(src);
}

KSurface::KSurface(const KSurface& src) : KSurface() {
	if (src.m_shared != nullptr) {
		m_shared = src.m_shared;
		m_shared->refCount += 1;
		x = src.x;
		y = src.y;
		w = src.w;
		h = src.h;
	}
}

KSurface::KSurface(const std::string& fileName) : KSurface() {
	KLib_IMG::init();
	from(IMG_Load(fileName.c_str()));
}

KSurface::KSurface(int width, int height) : KSurface(
	SDL_CreateRGBSurface(
		0, width, height, 32,
		KColor{255, 0, 0, 0},
		KColor{0, 255, 0, 0},
		KColor{0, 0, 255, 0},
		KColor{0, 0, 0, 255}
	)
) {}

KSurface::KSurface(int width, int height, KColor color) : KSurface(width, height) {
	fill(color);
}

void KSurface::from(SDL_Surface* src) {
	clear();
	if (src != nullptr) {
		if (
			src->format->BitsPerPixel != 32 ||
			src->format->Rmask != KColor{255, 0, 0, 0} ||
			src->format->Gmask != KColor{0, 255, 0, 0} ||
			src->format->Bmask != KColor{0, 0, 255, 0} ||
			src->format->Amask != KColor{0, 0, 0, 255}
		) {
			SDL_Surface* tmp = SDL_ConvertSurface(src, KLib_SDL::format, 0);
			SDL_FreeSurface(src);
			src = tmp;
		}
		m_shared = new Shared;
		m_shared->surface = src;
		m_shared->refCount = 1;
		m_shared->id = (unsigned int)-1;
		w = src->w;
		h = src->h;
	}
}

KSurface::~KSurface() {
	clear();
}

unsigned int KSurface::get_id() const {
	if (m_shared != nullptr) {
		if (m_shared->id == (unsigned int)-1) {
			m_shared->id = KLib_SDL::linker->new_id();
		}
		return m_shared->id;
	}
	return (unsigned int)-1;
}

bool KSurface::unlock_write(bool force) {
	if (m_shared != nullptr) {
		if (m_shared->refCount > 1) {
			Shared* tmp = new Shared;
			tmp->surface = SDL_ConvertSurface(tmp->surface, tmp->surface->format, 0);
			tmp->refCount = 1;
			tmp->id = (unsigned int)-1;
			m_shared->refCount -= 1;
			m_shared = tmp;
			return true;
		}
		else {
			if (m_shared->id != (unsigned int)-1) {
				if (force) {
					KLib_SDL::linker->del_id(m_shared->id);
					m_shared->id = (unsigned int)-1;
					return true;
				}
				return false;
			}
			return true;
		}
	}
	return false;
}

unsigned int KSurface::size() const {
	return w * h;
}

SDL_Surface* KSurface::get_ptr() const {
	if (m_shared == nullptr) return nullptr;
	return m_shared->surface;
}

void KSurface::clear() {
	if (m_shared != nullptr) {
		m_shared->refCount -= 1;
		if (m_shared->refCount == 0) {
			if (m_shared->id != (unsigned int)-1) {
				KLib_SDL::linker->del_id(m_shared->id);
			}
			SDL_FreeSurface(m_shared->surface);
			delete m_shared;
		}
		m_shared = nullptr;
	}
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

KSurface& KSurface::operator=(const KSurface& src) {
	if (src.m_shared != nullptr) {
		src.m_shared->refCount += 1;
	}
	clear();
	m_shared = src.m_shared;
	w = src.w;
	h = src.h;
	x = src.x;
	y = src.y;
	return *this;
}

void KSurface::fill(const KRect<>& rect, KColor color) {
	if (unlock_write()) {
		SDL_FillRect(m_shared->surface, (SDL_Rect*)&rect, color);
	}
}

void KSurface::fill(KColor color) {
	if (unlock_write()) {
		SDL_FillRect(m_shared->surface, nullptr, color);
	}
}

void KSurface::print(const KSurface& src, int x, int y) {
	if (src.m_shared != nullptr && unlock_write()) {
		SDL_Rect sdlRect = {x, y, 0, 0};
		SDL_BlitSurface(src.m_shared->surface, nullptr, m_shared->surface, &sdlRect);
	}
}

KColor KSurface::get_pixel(int x, int y) const {
	if (m_shared == nullptr) return KColor{0, 0, 0, 0};
	return ((KColor*)m_shared->surface->pixels)[y * w + x];
}

void KSurface::set_pixel(int x, int y, KColor color) {
	if (unlock_write()) {
		((KColor*)m_shared->surface->pixels)[y * w + x] = color;
	}
}

void KSurface::save(const std::string& fileName) const {
	if (m_shared != nullptr) {
		SDL_SaveBMP(m_shared->surface, fileName.c_str());
	}
}

KSurface KSurface::smaller_copy(int factor) const {
	KSurface smallerDst(w / factor, h / factor);
	for (int x = 0; x < smallerDst.w; x++) {
		for (int y = 0; y < smallerDst.h; y++) {
			smallerDst.set_pixel(x, y, get_pixel(x * factor + factor / 2, y * factor + factor / 2));
		}
	}
	return smallerDst;
}

KSurface KSurface::bigger_copy(int factor) const {
	KSurface dst(w * factor, h * factor);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			for (int xB = 0; xB < factor; xB++) {
				for (int yB = 0; yB < factor; yB++) {
					dst.set_pixel(x * factor + xB, y * factor + yB, get_pixel(x, y));
				}
			}
		}
	}
	return dst;
}

void KSurface::blur(int radius) {
	if (m_shared ==  nullptr || not unlock_write()) {
		return;
	}
	KColor* src = get_map();
	KColor* dst = new KColor[w * h];
	int surface = radius * 2;
	KColor color;
	for (int y = 0; y < h; y++) {
		int r = 0, g = 0, b = 0, a = 0;
		color = src[y * w];
		r += color.r * radius;
		g += color.g * radius;
		b += color.b * radius;
		a += color.a * radius;
		for (int x = 0; x < radius; x++) {
			color = src[y * w + ((x >= w) ? (w - 1) : x)];
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
		}
		for (int x = 0; x < w; x++) {
			dst[y * w + x].r = r / surface;
			dst[y * w + x].g = g / surface;
			dst[y * w + x].b = b / surface;
			dst[y * w + x].a = a / surface;
			color = src[y * w + ((x + radius >= w) ? (w - 1) : (x + radius))];
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
			color = src[y * w + ((x - radius < 0) ? 0 : (x - radius))];
			r -= color.r;
			g -= color.g;
			b -= color.b;
			a -= color.a;
		}
	}
	std::swap(src, dst);
	for (int x = 0; x < w; x++) {
		int r = 0, g = 0, b = 0, a = 0;
		color = src[x];
		r += color.r * radius;
		g += color.g * radius;
		b += color.b * radius;
		a += color.a * radius;
		for (int y = 0; y < radius; y++) {
			color = src[((y >= h) ? (h - 1) : y) * w + x];
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
		}
		for (int y = 0; y < h; y++) {
			dst[y * w + x].r = r / surface;
			dst[y * w + x].g = g / surface;
			dst[y * w + x].b = b / surface;
			dst[y * w + x].a = a / surface;
			color = src[((y + radius >= h) ? (h - 1) : (y + radius)) * w + x];
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
			color = src[((y - radius < 0) ? 0 : (y - radius)) * w + x];
			r -= color.r;
			g -= color.g;
			b -= color.b;
			a -= color.a;
		}
	}
	delete src;
}

void KSurface::blur_strong(int factor) {
	assert(factor > 0 && factor <= 16);
	if (factor > 1) {
		*this = smaller_copy(16);
		blur(2 * factor);
		*this = bigger_copy(4);
		blur(3 * factor);
		*this = bigger_copy(2);
		blur(factor);
		*this = bigger_copy(2);
	} else {
		*this = smaller_copy(8);
		blur(2 * factor);
		*this = bigger_copy(2);
		blur(3 * factor);
		*this = bigger_copy(2);
		blur(2 * factor);
		*this = bigger_copy(2);
	}
}

void KSurface::blur_quick(int factor) {
	assert(factor > 0 && factor <= 16);
	if (factor < 12) {
		factor+= 2;
		*this = smaller_copy(16);
		blur(factor);
		*this = bigger_copy(4);
		blur(2 * factor);
		*this = bigger_copy(4);
	} else {
		*this = smaller_copy(32);
		blur(factor / 2);
		*this = bigger_copy(4);
		blur(factor);
		*this = bigger_copy(8);
	}
}

void KSurface::darker(unsigned char factor) {
	KColor* map = get_map();
	for (int i = 0; i < w * h; i++) {
		map[i].r = map[i].r * factor / 255;
		map[i].g = map[i].g * factor / 255;
		map[i].b = map[i].b * factor / 255;
	}
}

KColor* KSurface::operator[](int y) {
	if (unlock_write()) {
		return ((KColor*)m_shared->surface->pixels) + y * w;
	}
	return nullptr;
}

const KColor* KSurface::operator[](int y) const {
	if (m_shared == nullptr) return nullptr;
	return ((KColor*)m_shared->surface->pixels) + y * w;
}

KColor* KSurface::get_map() {
	if (m_shared == nullptr || not unlock_write()) return nullptr;
	return (KColor*)m_shared->surface->pixels;
}

const KColor* KSurface::get_map() const {
	if (m_shared ==nullptr) return nullptr;
	return (const KColor*)m_shared->surface->pixels;
}
