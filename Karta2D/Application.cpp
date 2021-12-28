#include "Application.h"

Application* Application::instance = nullptr;

Application* Application::Instance() {
	if (instance == nullptr)
		instance = new Application();

	return instance;
}

void Application::release() {
	delete instance;
	instance = nullptr;
}

Application::Application() {
	quit = false;

	graphics = Graphics::Instance();
	timer = Timer::Instance();

	ent.AddComponent<Transform2D>();
	ent.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2));
	ent.GetComponent<Transform2D>()->setRotation(0);

	ent.AddComponent<Square>();
	ent.GetComponent<Square>()->setSize(100, 100);
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	ent.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(20, -10));
	ent.GetComponent<Transform2D>()->rotate(10 * timer->Instance()->getDeltaTime());

	ent.update();
}

void Application::lateUpdate() {
	
}

void Application::render() {

	graphics->clear();

	////RENDER ENTITIES HERE////

	ent.render();

	///////////////////////////

	graphics->render();
}

void Application::run() {

	while (!quit) {

		timer->update();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;
		}
		
		if (timer->getDeltaTime() >= 1.0f / FRAME_RATE) {

			earlyUpdate();
			update();
			lateUpdate();
			render();
		}
	}
}

void Application::exit() {
	graphics->exit();
}