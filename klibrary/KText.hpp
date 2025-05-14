#ifndef KText_HEADER_DEFINED
#define KText_HEADER_DEFINED

#include <string>
#include "KSurface.hpp"
#include "KFont.hpp"
#include "KColor.hpp"

/// \brief A class that regroup a string, a KFont and a KColor into a KSurface.
///
class KText : public KSurface {
	std::string m_str;
	KFont m_font;
	KColor m_color;
public:
	/// \brief Initialize the class.
	///
	KText();

	/// \brief Initialize the class and set its attributes.
	///
	KText(std::string const& str, const KFont& font, KColor color);

	/// \brief Initialize the class and set its attributes.
	///
	KText(const char* str, const KFont& font, KColor color);

	void assign(std::string const& str, const KFont& font, KColor color);
	void assign(const char* str, const KFont& font, KColor color);

	/// \brief Set the string without updating the surface.
	///
	void set(std::string const& str);

	/// \brief Set the string without updating the surface.
	///
	void set(const char* str);

	/// \brief Set the font without updating the surface.
	///
	void set(KFont& font);

	/// \brief Set the color without updating the surface.
	///
	void set(KColor color);

	/// \brief Update the surface.
	///
	void update();

	/// \brief Set the string and update the surface.
	///
	void change(std::string const& str);

	/// \brief Set the string and update the surface.
	///
	void change(const char* str);

	/// \brief Set the font and update the surface.
	///
	void change(KFont& font);

	/// \brief Set the color and update the surface.
	///
	void change(KColor color);
};

#endif
