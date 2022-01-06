#pragma once
#include "SimpleECS.h"
#include "EntityManager.h"
#include "Components.h"
#include "Collision2D.h"
#include "Graphics.h"
#include "Timer.h"

class Application {
public:
	static Application* Instance();
	void run();
	void exit();

private:
	static void release();

	Application();
	~Application();

	void earlyUpdate();
	void update();
	void lateUpdate();
	void render();

private:
	bool simulate = false;
	static Application* instance;
	const int FRAME_RATE = 60;
	bool quit;

	SDL_Event event;
	Graphics* graphics;
	Timer* timer;

	Entity box, circle, ground;
};