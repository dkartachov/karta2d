#pragma once
#include "SimpleECS/SimpleECS.h"
#include "SimpleECS/EntityManager.h"
#include "SimpleECS/Components.h"
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

	Entity whiteBox;
	Entity redBox;
	Entity blueBox;
	Entity leftWall, rightWall, floor, ceiling;
};