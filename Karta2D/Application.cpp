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
	ent.AddComponent<Square>();

	ent.GetComponent<Square>()->setSize(100, 100);
	ent.GetComponent<Square>()->setColor(100, 255, 100);
	ent.GetComponent<Transform2D>()->setScale(Vector2D(1, 1.2));
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	//ent.GetComponent<Square>()->setSize(ent.GetComponent<Square>()->getSize().x + 1, ent.GetComponent<Square>()->getSize().y + 1);
	ent.GetComponent<Transform2D>()->setPosition(ent.GetComponent<Transform2D>()->getPosition() + oneVector);
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