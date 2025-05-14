#ifndef KRect_HEADER_DEFINED
#define KRect_HEADER_DEFINED

#include <iostream>
#include "KLib_SDL.hpp"

/// \brief A struct that represent a rectangle.
///
/// \tparam T the type of the X and Y coordinates
/// \tparam U the type of the dimensions variables
template<typename T = int, typename U = T>
struct KRect {
	/// brief The X coordinate.
	T x;
	/// brief The Y coordinate.
	T y;

	/// \brief The width.
	///
	U w;

	/// \brief The height.
	///
	U h;

	/// \brief Constructs a centered rectangle.
	///
	/// Its position is its center
	template<typename X, typename Y, typename W, typename H>
	static KRect<X, W> centered(X x, Y y, W w, H h) {
		return {x - w / 2, y - h / 2, w, h};
	}

	/// \brief Initializes the class.
	///
	KRect(): x(0), y(0), w(0), h(0) {}

	/// \brief Default constructor.
	///
	template<typename X, typename Y, typename W, typename H>
	KRect(X x, Y y, W w, H h): x(x), y(y), w(w), h(h) {}

	/// \brief Copy constructor.
	///
	template<typename V, typename W>
	KRect(const KRect<V, W>& src): x(src.x), y(src.y), w(src.w), h(src.h) {}

	/// \brief Tests if the rectangle is on the specified position.
	///
	bool is_on(T x_, T y_) {
		return x_ >= x && x_ < x + w && y_ >= y && y_ < y + h;
	}

	/// \brief Tests if two rectangle are overlaping each other.
	///
	template<typename V, typename W>
	bool is_on(const KRect<V, W>& rect) {
		return rect.x + rect.w > x && rect.x < x + w && rect.y + rect.h > y && rect.y < y + h;
	}

	/// \brief Assignement operator.
	///
	template<typename V, typename W>
	KRect<T, U>& operator=(const KRect<V, W>& copy) {
		x = copy.x;
		y = copy.y;
		w = copy.w;
		h = copy.h;
	}

	/// \brief Converter to SDL_Rect.
	///
	operator SDL_Rect() const {
		return {x, y, w, h};
	}
};

/// \brief Prints to output stream as "(x, y, w, h)".
///
template<typename T, typename U = T>
std::ostream& operator<<(std::ostream& ostream, KRect<T, U>& rect)
{
	return ostream << "(" << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << ")";
}

/// \brief Loads from input stream as "(x, y, w, h)".
///
template<typename T, typename U = T>
std::istream& operator>>(std::istream& istream, KRect<T, U>& rect)
{
	char tmp;
	return istream >> tmp >> rect.x >> tmp >> rect.y >> tmp >> rect.w >> tmp >> rect.h >> tmp;
}

#endif
