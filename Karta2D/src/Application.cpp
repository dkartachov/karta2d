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
	box.GetComponent<Box>()->setSize(60, 60);
	box.GetComponent<Box>()->fill();
	box.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box.AddComponent<Rigidbody2D>();
	box.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	box.GetComponent<BoxCollider2D>()->setSize(60, 60);
	box.GetComponent<Rigidbody2D>()->setMass(1);
	box.GetComponent<Rigidbody2D>()->setVelocity({ 0, -200 });
	box.GetComponent<Rigidbody2D>()->setGravity(true);

	circle.setName("Circle");
	circle.AddComponent<CircleCollider2D>();
	circle.AddComponent<Rigidbody2D>();
	circle.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 140, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	circle.GetComponent<CircleCollider2D>()->setRadius(60);
	circle.GetComponent<Rigidbody2D>()->setMass(1);
	circle.GetComponent<Rigidbody2D>()->setVelocity({ -100, -200});
	circle.GetComponent<Rigidbody2D>()->setGravity(true);

	ground.setName("Ground");
	ground.AddComponent<BoxCollider2D>();
	ground.AddComponent<Box>();
	ground.GetComponent<Box>()->setSize(1600, 50);
	ground.GetComponent<Box>()->fill();
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 300));
	ground.GetComponent<BoxCollider2D>()->setSize(1600, 50);

	std::vector<Entity*> entities = { &ground, &box, &circle };
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

	//Collision2D::BoxCircle(box, circle);
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