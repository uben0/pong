#ifndef KLib_SDL_HEADER_DEFINED
#define KLib_SDL_HEADER_DEFINED

#include <vector>
#include <SDL2/SDL.h>


class KWindow;
namespace KLib_SDL {
	extern SDL_Surface*    surface;
	extern SDL_PixelFormat* format;
	void initialize()   __attribute__ ((constructor));
	void uninitialize() __attribute__ ((destructor ));
	class Linker {
		unsigned int m_firstFree;
		std::vector<bool> m_ids;
		std::vector<KWindow*> m_windows;
	public:
		Linker();
		unsigned int new_id();
		void del_id(unsigned int id);
		void add_window(KWindow* window);
		void remove_window(KWindow* window);
		void update_events();
	};
	extern Linker* linker;
}

#endif
