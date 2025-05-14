#ifndef KColor_HEADER_DEFINED
#define KColor_HEADER_DEFINED

#include <SDL2/SDL.h>
#include <iostream>

/// \brief A class that holds an RGBA color.
///
struct KColor {
	/// \brief Red channel (0-255).
	///
	unsigned char r;

	/// \brief Green channel (0-255).
	///
	unsigned char g;

	/// \brief Blue channel (0-255).
	///
	unsigned char b;

	/// \brief Alpha channel (0-255).
	///
	unsigned char a;

	/// \brief Initialize to 0x00000000 (black).
	///
	KColor();

	/// \brief Initialize with an unsigned long integer.
	///
	/// This is for hexadecimal color (0xFF1200)
	/// \param rgba the color in an integer
	KColor(unsigned long rgba);

	/// \brief Initialize with the 4 channels.
	///
	/// \param r the red channel
	/// \param g the green channel
	/// \param b the blue channel
	/// \param a the alpha channel
	KColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	/// \brief Convert to SDL_Color.
	///
	SDL_Color to_SDL_Color() const;

	/// \brief Converter to unsigned long integer
	///
	inline operator Uint32() const {
		return *(Uint32*)this;
	}

	/// \brief Predifiened white color.
	///
	static const KColor White;

	/// \brief Predifiened black color.
	///
	static const KColor Black;

	/// \brief Predifiened red color.
	///
	static const KColor Red;

	/// \brief Predifiened green color.
	///
	static const KColor Green;

	/// \brief Predifiened blue color.
	///
	static const KColor Blue;

	/// \brief Predifiened cyan color.
	///
	static const KColor Cyan;

	/// \brief Predifiened magenta color.
	///
	static const KColor Magenta;

	/// \brief Predifiened yellow color.
	///
	static const KColor Yellow;

	/// \brief Predifiened transparent color.
	///
	static const KColor Transparent;

	KColor& operator+=(KColor added);
};

/// \brief Operator << for output stream.
///
std::ostream& operator<<(std::ostream& ostream, const KColor& color);

/// \brief Operator >> for input stream.
///
std::istream& operator>>(std::istream& istream,       KColor& color);

#endif
