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

	box.setName("Box 1");
	box.AddComponent<Transform2D>();
	box.AddComponent<BoxCollider2D>();
	//box.AddComponent<Box>();
	//box.GetComponent<Box>()->setSize(1, 0.2);
	//box.GetComponent<Box>()->fill();
	//box.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box.AddComponent<Rigidbody2D>();
	box.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2.0 + 100, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200.0), true);
	box.GetComponent<BoxCollider2D>()->setSize(1, 0.2);
	box.GetComponent<Rigidbody2D>()->setMass(1);
	box.GetComponent<Rigidbody2D>()->setVelocity({ -2, -1 });
	box.GetComponent<Rigidbody2D>()->setAngularSpeed(180);
	box.GetComponent<Transform2D>()->setRotation(0);
	//box.GetComponent<Rigidbody2D>()->setGravity(false);

	box2.setName("Box 2");
	box2.AddComponent<Transform2D>();
	box2.AddComponent<BoxCollider2D>();
	//box2.AddComponent<Box>();
	//box2.GetComponent<Box>()->setSize(1, 0.2);
	//box2.GetComponent<Box>()->fill();
	//box2.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box2.AddComponent<Rigidbody2D>();
	box2.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2.0 - 100, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200.0), true);
	box2.GetComponent<BoxCollider2D>()->setSize(0.5, 0.5);
	box2.GetComponent<Rigidbody2D>()->setMass(2);
	box2.GetComponent<Rigidbody2D>()->setVelocity({ -2, -2 });
	box2.GetComponent<Rigidbody2D>()->setAngularSpeed(360);
	box2.GetComponent<Transform2D>()->setRotation(0);
	//box2.GetComponent<Rigidbody2D>()->setGravity(false);

 	ground.setName("Ground");
	ground.AddComponent<Transform2D>();
	ground.AddComponent<BoxCollider2D>();
	//ground.AddComponent<Box>();
	//ground.GetComponent<Box>()->setSize(6, 0.5);
	//ground.GetComponent<Box>()->fill();
	//ground.GetComponent<Box>()->setColor(0, 0, 255, 255);
	//ground.AddComponent<Rigidbody2D>();
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2), true);
	ground.GetComponent<BoxCollider2D>()->setSize(6, 0.5);
	//ground.GetComponent<Rigidbody2D>()->setMass(400);
	//ground.GetComponent<Rigidbody2D>()->setVelocity({ 0, 0 });
	//ground.GetComponent<Rigidbody2D>()->setAngularSpeed(30);
	//ground.GetComponent<Transform2D>()->setRotation(0);
	//ground.GetComponent<Rigidbody2D>()->setGravity(false);

	wall.setName("Wall");
	wall.AddComponent<Transform2D>();
	wall.AddComponent<BoxCollider2D>();
	//wall.AddComponent<Box>();
	//wall.GetComponent<Box>()->setSize(6, 0.5);
	//wall.GetComponent<Box>()->fill();
	//wall.GetComponent<Box>()->setColor(0, 0, 255, 255);
	//wall.AddComponent<Rigidbody2D>();
	wall.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2.0 - 325.0, Graphics::Instance()->SCREEN_HEIGHT / 2 - 125.0), true);
	wall.GetComponent<BoxCollider2D>()->setSize(0.5, 3);
	//wall.GetComponent<Rigidbody2D>()->setMass(400);
	//wall.GetComponent<Rigidbody2D>()->setVelocity({ 0, 0 });
	//wall.GetComponent<Rigidbody2D>()->setAngularSpeed(30);
	wall.GetComponent<Transform2D>()->setRotation(0);
	//wall.GetComponent<Rigidbody2D>()->setGravity(false);

	std::vector<Entity*> entities = { &box, &ground, &box2, &wall };
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