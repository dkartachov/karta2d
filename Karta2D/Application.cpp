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
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	Transform2D* transform = ent.GetComponent<Transform2D>();

	Vector2D position = transform->getPosition();
	position.x++;
	position.y++;

	transform->setPosition(position);
	transform->toString();
}

void Application::lateUpdate() {
	
}

void Application::render() {

	graphics->clear();

	////RENDER ENTITIES HERE////



	///////////////////////////

	graphics->render();
}

void Application::run() {

	while (!quit) {

		timer->update();

		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				quit = true;

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