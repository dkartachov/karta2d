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

	box.setName("red box");
	box.AddComponent<BoxCollider2D>();
	box.AddComponent<Box>();
	box.GetComponent<Box>()->setSize(120, 60);
	box.GetComponent<Box>()->fill();
	box.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box.AddComponent<Rigidbody2D>();
	box.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 100, Graphics::Instance()->SCREEN_HEIGHT / 2 - 50));
	box.GetComponent<BoxCollider2D>()->setSize(120, 60);
	box.GetComponent<Rigidbody2D>()->setMass(1);
	box.GetComponent<Rigidbody2D>()->setVelocity({ -200, 0 });
	box.GetComponent<Transform2D>()->setRotation(-45);
	box.GetComponent<Rigidbody2D>()->setAngularSpeed(0);

	bbox.setName("red box");
	bbox.AddComponent<BoxCollider2D>();
	bbox.AddComponent<Box>();
	bbox.GetComponent<Box>()->setSize(60, 60);
	bbox.GetComponent<Box>()->fill();
	bbox.GetComponent<Box>()->setColor(255, 0, 255, 255);
	bbox.AddComponent<Rigidbody2D>();
	bbox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 200, Graphics::Instance()->SCREEN_HEIGHT / 2 - 50));
	bbox.GetComponent<BoxCollider2D>()->setSize(60, 60);
	bbox.GetComponent<Rigidbody2D>()->setMass(1);
	bbox.GetComponent<Rigidbody2D>()->setVelocity({ -200, 0 });
	bbox.GetComponent<Transform2D>()->setRotation(-45);
	bbox.GetComponent<Rigidbody2D>()->setAngularSpeed(0);

	ground.setName("Ground");
	ground.AddComponent<BoxCollider2D>();
	ground.AddComponent<Box>();
	ground.GetComponent<Box>()->setSize(600, 50);
	ground.GetComponent<Box>()->fill();
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	ground.GetComponent<BoxCollider2D>()->setSize(600, 50);
	ground.GetComponent<Transform2D>()->setRotation(-45);
	ground.GetComponent<BoxCollider2D>()->restitution = 1;

	std::vector<Entity*> entities = { &bbox, &ground, &box };
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

	EntityManager::getInstance().render();

	graphics->render();
}

void Application::run() {
	while (!quit) {

		timer->update();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					simulate = simulate ? false : true;
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