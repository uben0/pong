#ifndef KFont_HEADER_DEFINED
#define KFont_HEADER_DEFINED

#include <string>
#include "KLib_TTF.hpp"
#include "KSurface.hpp"
#include "KColor.hpp"

/// \brief Class thet hold a font.
///
class KFont {
	struct Shared {
		TTF_Font* font;
		unsigned int refCount;
	};
	Shared* m_shared;
public:
	/// \brief Initialize the class.
	///
	KFont();

	/// \brief Initialize the class and call open()
	///
	KFont(const std::string& file, int size);

	/// \brief Copy the font.
	///
	KFont(const KFont& font);

	/// \brief Destructor
	///
	~KFont();

	/// \brief Open a ttf font file.
	///
	/// \param file the name of the ttf file
	/// \param size the size of the font
	bool open(const std::string& file, int size);

	/// \brief Close the font.
	///
	void close();

	/// \brief Test whther the font is opened or not.
	///
	bool is_opened() const;

	/// \brief test whether the font is closed or not.
	///
	bool is_closed() const;

	/// \brief Generate a KSurface with rendered text.
	///
	/// \param text the text the render
	/// \param color the color to render the text with
	KSurface operator()(const std::string& text, KColor color);

	/// \brief Assign a font.
	///
	/// Close the lvalue and copy the rvalue.
	KFont& operator=(const KFont& font);
};

#endif
