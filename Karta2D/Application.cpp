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

	box.AddComponent<Transform2D>();
	box.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 50, Graphics::Instance()->SCREEN_HEIGHT / 2));
	box.GetComponent<Transform2D>()->setRotation(0);

	box.AddComponent<Box>();
	box.GetComponent<Box>()->setSize(100, 50);
	box.GetComponent<Box>()->fill();

	circle.AddComponent<Circle>();
	circle.GetComponent<Transform2D>()->setPosition(box.GetComponent<Transform2D>()->getPosition() - Vector2D(100, 0));
	circle.GetComponent<Circle>()->setRadius(50);
	circle.GetComponent<Circle>()->setOutline(2);
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	box.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(20, -20));
	box.GetComponent<Transform2D>()->rotate(25 * timer->Instance()->getDeltaTime());

	circle.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(-30, 20));

	box.update();
	circle.update();
}

void Application::lateUpdate() {
	
}

void Application::render() {

	graphics->clear();

	////RENDER ENTITIES HERE////

	box.render();
	circle.render();

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