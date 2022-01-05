#include "Timer.h"

Timer* Timer::instance = nullptr;

Timer* Timer::Instance() {
	if (instance == nullptr)
		instance = new Timer();
	
	return instance;
}

void Timer::release() {
	delete instance;
	instance = nullptr;
}

Timer::Timer() {
	reset();
	
	elapsedTicks = 0.0f;
	deltaTime = 0.0f;
	timeScale = 1.0f;
}

void Timer::reset() {
	startTicks = SDL_GetTicks();
}

float Timer::getDeltaTime() {
	return deltaTime;
}

void Timer::setTimeScale(float t) {
	timeScale = t;
}

float Timer::getTimeScale() {
	return timeScale;
}

void Timer::update() {
	elapsedTicks = SDL_GetTicks() - startTicks;
	deltaTime = elapsedTicks * 0.001f;
}