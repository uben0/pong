#ifndef KWindow_HEADER_DEFINED
#define KWindow_HEADER_DEFINED

#include <string>

#include "KLib_SDL.hpp"
#include "KRect.hpp"
#include "KColor.hpp"
#include "KSurface.hpp"
#include "KEvent.hpp"

/// \brief Class holding a window.
///
class KWindow : public KRect<> {
	friend class KLib_SDL::Linker;
	friend class KEvent;
	SDL_Window*               m_window;
	SDL_Renderer*             m_renderer;
	std::string               m_name;
	bool                      m_hidden;
	unsigned int              m_id;
	std::vector<SDL_Texture*> m_textures;

	void verbose(const std::string& message);
	void remove_texture(unsigned int id);
	void update_size();
public:
	/// \brief Store occuring events.
	///
	KEvent event;

	/// \brief A type representing flags given to a window.
	///
	/// Flags combine together with the operator |
	typedef Uint32 Flags;
	/// \brief Flag to open hidden window.
	///
	static constexpr Flags Hidden    = SDL_WINDOW_HIDDEN;
	/// \brief Flag to open resizable window.
	///
	static constexpr Flags Resizable = SDL_WINDOW_RESIZABLE;

	/// \brief Initialize the class.
	///
	KWindow();

	/// \brief Initialize the class and call open()
	///
	KWindow(const std::string& name, int width, int height, const KSurface& icon = {}, Flags flags = 0);

	/// \brief Destructor.
	///
	~KWindow();

	/// \brief Open the window
	///
	/// \param name the name of the window
	/// \param width the width of the window
	/// \param height the height of the window
	/// \param icon the icon of the window
	/// \param flags options can be Hidden and/or Resizable
	/// \return if the window have been created, it returns true, and false otherwise
	bool open(const std::string& name, int width, int height, const KSurface& icon = {}, Flags flags = 0);

	/// \brief Close the window if it is opened
	///
	void close();

	/// \brief Enable or disable the alpha operations.
	///
	/// By default, the alpha blend is disabled.
	/// \param set true to enable and false to disable
	void set_alpha_blend(bool set);

	/// \brief Resize the window.
	///
	/// \param w the new width of the window
	/// \param h the new height of the window
	void resize(int w, int h);

	/// \brief Preload the surface in VRAM.
	///
	/// This operation is automatic but it can be done to prevent lag on the first
	/// rendered frame.
	void load_texture(KSurface& surface);

	/// \brief Get the window id.
	///
	/// \return The window id.
	unsigned int get_id() const;

	/// \brief Indicates whether the window is opened or not.
	///
	bool is_opened();
	/// \brief Indicates whether the window is closed or not.
	///
	bool is_closed();

	/// \brief Show the window if it was hidden.
	///
	void show();
	/// \brief Hide the window if it was visible.
	///
	void hide();

	/// \brief Fill the entire window.
	///
	/// \param color the color to fill with
	void fill(KColor color);
	/// \brief Fill a rectangular zone on the window.
	///
	/// \param rect the rectangle to fill
	/// \param color the color to fill with
	void fill(const KRect<>& rect, KColor color);
	/// \brief Draw a rectangle on the window.
	///
	/// \param rect the rectangle to draw
	/// \param color the color to draw with
	void draw_rect(const KRect<>& rect, KColor color);
	/// \brief Draw a line on the window.
	///
	/// \param x1 the x coordinate of start point
	/// \param y1 the y coordinate of start point
	/// \param x2 the x coordinate of end point
	/// \param y2 the y coordinate of end point
	/// \param color the color to draw with
	void draw_line(int x1, int y1, int x2, int y2, KColor color);

	/// \brief Print a KSurface on the window.
	///
	/// \param src the surface to print
	void print(KSurface& src);
	/// \brief Print a KSurface on the window.
	///
	/// \param src the surface to print
	/// \param x overwrite the x atribute of the surface
	/// \param y overwrite the y atribute of the surface
	void print(KSurface& src, int x, int y);
	/// \brief Print a KSurface on the window.
	///
	/// \param src the surface to print
	/// \param x overwrite the x atribute of the surface
	/// \param y overwrite the y atribute of the surface
	/// \param w overwrite the w atribute of the surface
	/// \param h overwrite the h atribute of the surface
	void print(KSurface& src, int x, int y, int w, int h);

	/// \brief Update the window with the new rendered frame.
	///
	void display();
};

#endif
