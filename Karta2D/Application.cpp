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

	parentBox.setName("Parent");
	parentBox.AddComponent<Box>();
	parentBox.GetComponent<Box>()->setSize(100, 100);
	parentBox.GetComponent<Box>()->fill();
	parentBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2));
	//parentBox.setChild(&childBox);

	childBox.setName("Child");
	childBox.AddComponent<Box>();
	childBox.GetComponent<Box>()->setSize(50, 50);
	childBox.GetComponent<Box>()->setColor(0, 0, 0, 255);
	childBox.GetComponent<Box>()->fill();
	childBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 200, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200));
	//childBox.setChild(&childChildBox);

	childChildBox.setName("childChild");
	childChildBox.AddComponent<Box>();
	childChildBox.GetComponent<Box>()->setSize(50, 50);
	childChildBox.GetComponent<Box>()->setColor(255, 0, 0, 255);
	childChildBox.GetComponent<Box>()->fill();
	childChildBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 200, Graphics::Instance()->SCREEN_HEIGHT / 2 + 200));

	parentBox.addChildren({ &childBox, &childChildBox });
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	//box.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(20, -20));
	//childBox.GetComponent<Transform2D>()->rotate(25 * timer->Instance()->getDeltaTime());
	parentBox.GetComponent<Transform2D>()->rotate(25 * timer->Instance()->getDeltaTime());
	//parentBox.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(30, 0));

	parentBox.update();
	childBox.update();
	childChildBox.update();
}

void Application::lateUpdate() {
	
}

void Application::render() {

	graphics->clear();

	////RENDER ENTITIES HERE////

	parentBox.render();
	childBox.render();
	childChildBox.render();

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