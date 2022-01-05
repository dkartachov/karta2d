#pragma once
#include <SDL.h>

class Timer {
public:
	static Timer* Instance();
	static void release();

	Timer();

	float getDeltaTime();
	float getTimeScale();

	void setTimeScale(float t);

	void update();
	void reset();
private:
	static Timer* instance;
	unsigned int startTicks, elapsedTicks;
	float deltaTime, timeScale;
};