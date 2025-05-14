#ifndef KLib_IMG_HEADER_DEFINED
#define KLib_IMG_HEADER_DEFINED

#include <SDL2/SDL_image.h>

namespace KLib_IMG {
	extern bool active;
	void init();
	void quit() __attribute__ ((destructor ));
}

#endif
