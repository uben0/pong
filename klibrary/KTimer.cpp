#include <SDL2/SDL.h>
#include <sstream>
#include <iomanip>
#include "KTimer.hpp"

KTimer::KTimer(unsigned int msPerLoop): mMsPerLoop(msPerLoop), mCount(0), mStart(SDL_GetTicks()) {
}

void KTimer::wait() {
	mCount++;
	Uint32 gotTicks    = SDL_GetTicks();
	Uint32 loopEndTime = mMsPerLoop * mCount + mStart;
	if (gotTicks < loopEndTime) {
		SDL_Delay(loopEndTime - gotTicks);
	}
}

void KTimer::wait_for(unsigned int msToWait) {
	SDL_Delay(msToWait);
}

std::string KTimer::date() {
	std::stringstream buff;
	std::time_t currentTime = std::time(nullptr);
	buff << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
	return buff.str();
}

void KTimer::start(unsigned int msPerLoop) {
	mStart = SDL_GetTicks();
	mCount = 0;
	mMsPerLoop = (msPerLoop);
}

void KTimer::start() {
	start(mMsPerLoop);
}

unsigned long long KTimer::get_time() const {
	return SDL_GetTicks() - mStart;
}
