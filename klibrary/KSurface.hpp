#ifndef KSurface_HEADER_DEFINED
#define KSurface_HEADER_DEFINED

#include "KLib_SDL.hpp"
#include "KLib_IMG.hpp"
#include "KColor.hpp"
#include "KRect.hpp"

/// \brief Class that store a surface of pixels.
///
class KSurface : public KRect<> {
	struct Shared {
		SDL_Surface* surface;
		unsigned int refCount;
		unsigned int id;
	};
	Shared* m_shared;
public:
	/// \brief Unlock the surface for writing.
	///
	/// This operation is automaticc. It should only be called after that the
	/// surface have been loaded in VRAM to perfome a modification on it.
	/// Writing methods of the surface could call this function directly with
	/// force option but it would hide a potentially weighty operation.
	/// \param force if force is set on true, this function can not fail
	/// \return Whether the function succed or not.
	bool unlock_write(bool force = false);

	/// \brief Initialize the class.
	///
	KSurface();

	/// \brief Copy the surface
	///
	/// This operation does not duplicate the surface in memory. The surface
	/// will be duplicated only when a writing operation will be perfome on it.
	KSurface(const KSurface& src);

	/// \brief Initialize from an SDL_Surface.
	///
	/// The SDL_Surface will be keeped and should not be manually freed.
	KSurface(SDL_Surface* src);

	/// \brief Load image file.
	///
	KSurface(const std::string& fileName);

	/// \brief Create an empty surface.
	///
	/// \param width the width of the surface
	/// \param height the height of the surface
	KSurface(int width, int height);

	/// \brief Create an empty surface with color.
	///
	/// \param width the width of the surface
	/// \param height the height of the surface
	/// \param color the color to fill the surface with
	KSurface(int width, int height, KColor color);

	/// \brief Initialize from an SDL_Surface.
	///
	/// The SDL_Surface will be keeped and should not be manually freed.
	void from(SDL_Surface* src);

	/// \brief Destructor
	///
	~KSurface();

	/// \brief Get the pixel array.
	///
	KColor* get_map();

	/// \brief Get the read only pixel array.
	///
	const KColor* get_map() const;

	/// \brief Get the surface id.
	///
	unsigned int get_id() const;

	/// \brief Get the SDL_Surface behind.
	///
	/// This SDL_Surface should not be manually freed.
	SDL_Surface*  get_ptr() const;

	/// \brief Get the size of the surface.
	///
	/// It is equivalent to width * height.
	/// It is also the size of the array of pixels.
	unsigned int size() const;

	/// \brief Delete the surface.
	///
	void clear();

	/// \brief Fill the surface with a color.
	///
	/// \param color the color to fill the surface with
	void fill(KColor color);

	/// \brief Fill a rectangular zone with a color.
	///
	/// \param rect the rectangle to fill.
	/// \param color the color to fill the rectangle with
	void fill(const KRect<>& rect, KColor color);

	/// \brief Print à surface on the surface.
	///
	/// \param src the surface to print
	/// \param x the X coordinate where to print
	/// \param y the Y coordinate where to print
	void print(const KSurface& src, int x, int y);

	/// \brief Set a pixel.
	///
	/// \param x the X coordiante of the pixel
	/// \param y the Y coordiante of the pixel
	/// \param color the new color of the pixel
	void set_pixel(int x, int y, KColor color);

	/// \brief Get a pixel.
	///
	/// \param x the X coordiante of the pixel
	/// \param y the Y coordiante of the pixel
	/// \return color the color of the pixel
	KColor get_pixel(int x, int y) const;

	/// \brief Save the surface to a file.
	///
	/// It save image as bitmap (.bmp).
	/// \param fileName the name of the file
	void save(const std::string& fileName) const;

	/// \brief Make copy n times smaller.
	///
	/// \param factor the factor of reduction
	KSurface smaller_copy(int factor) const;

	/// \brief Make copy n times bigger.
	///
	/// \param factor the factor of enlargement
	KSurface bigger_copy( int factor) const;

	/// \brief Darken the surface.
	///
	/// \param factor the factor of darkening (0-255)
	void darker(unsigned char factor);

	/// \brief Blur the surface.
	///
	/// This function is quite slow and should only be used in a smart way.
	/// \param radius the raiud of the blur effect
	void blur(int radius);

	/// \brief Blur strongly the surface.
	///
	/// This function may reduce the size of the surface to match a multiple of
	/// 16 (new width = old width % 16, the same applies to height).
	/// \param factor an arbitrary factor for the effect magnitude.
	void blur_strong(int factor);

	/// \brief Blur quickly the surface.
	///
	/// This function may reduce the size of the surface to match a multiple of
	/// 32 (new width = old width % 32, the same applies to height).
	/// \param factor an arbitrary factor for the effect magnitude.
	void blur_quick(int factor);

	/// \brief Assign a surface.
	///
	/// Destroy the lvalue and copy the rvalue.
	KSurface&     operator=(const KSurface& src);

	/// \brief Access the umpteenth row of pixels.
	///
	KColor*       operator[](int y);

	/// \brief Access the umpteenth read only row of pixels.
	///
	const KColor* operator[](int y) const;
};

#endif
