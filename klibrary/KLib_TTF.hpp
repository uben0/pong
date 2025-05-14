#ifndef KLib_TTF_HEADER_DEFINED
#define KLib_TTF_HEADER_DEFINED

#include <SDL2/SDL_ttf.h>

namespace KLib_TTF {
	extern bool active;
	void init();
	void quit() __attribute__ ((destructor ));
}

#endif
