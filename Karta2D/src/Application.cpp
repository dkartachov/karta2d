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

	box.setName("Box");
	box.AddComponent<BoxCollider2D>();
	box.AddComponent<Box>();
	box.GetComponent<Box>()->setSize(30, 30);
	box.GetComponent<Box>()->fill();
	box.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box.AddComponent<Rigidbody2D>();
	box.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 200, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	box.GetComponent<BoxCollider2D>()->setSize(30, 30);
	box.GetComponent<Rigidbody2D>()->setMass(1);
	box.GetComponent<Rigidbody2D>()->setVelocity({ 150, -200 });

	bigBox.setName("Big Box");
	bigBox.AddComponent<BoxCollider2D>();
	bigBox.AddComponent<Box>();
	bigBox.GetComponent<Box>()->setSize(50, 60);
	bigBox.GetComponent<Box>()->fill();
	bigBox.GetComponent<Box>()->setColor(0, 0, 255, 255);
	bigBox.AddComponent<Rigidbody2D>();
	bigBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	bigBox.GetComponent<BoxCollider2D>()->setSize(50, 60);
	bigBox.GetComponent<Rigidbody2D>()->setMass(1);
	bigBox.GetComponent<Rigidbody2D>()->setVelocity({ -150, -200 });
	bigBox.GetComponent<Rigidbody2D>()->addForce({ 0, 0 });

	ground.setName("Ground");
	ground.AddComponent<BoxCollider2D>();
	ground.AddComponent<Box>();
	ground.GetComponent<Box>()->setSize(1600, 50);
	ground.GetComponent<Box>()->fill();
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 300));
	ground.GetComponent<BoxCollider2D>()->setSize(1600, 50);


	std::vector<Entity*> entities = { &ground, &box, &bigBox };
	EntityManager::getInstance().addEntities(entities);
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	if (!simulate) return;
	EntityManager::getInstance().update();
}

void Application::lateUpdate() {
	if (!simulate) return;
	Collision2D::resolveCollisions();
}

void Application::render() {

	graphics->clear();

	////RENDER ENTITIES HERE////

	EntityManager::getInstance().render();

	///////////////////////////

	graphics->render();
}

void Application::run() {
	float index = 0.1;
	while (!quit) {

		timer->update();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					simulate = true;
					break;
				}
			}
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