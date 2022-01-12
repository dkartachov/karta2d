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
	box.AddComponent<BoxCollider2D>();
	box.AddComponent<Box>();
	box.GetComponent<Box>()->setSize(1, 0.2);
	box.GetComponent<Box>()->fill();
	box.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box.AddComponent<Rigidbody2D>();
	box.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200), true);
	box.GetComponent<BoxCollider2D>()->setSize(1, 0.2);
	box.GetComponent<Rigidbody2D>()->setMass(1);
	box.GetComponent<Rigidbody2D>()->setVelocity({ -2, -2 });
	box.GetComponent<Rigidbody2D>()->setAngularSpeed(350);
	box.GetComponent<Transform2D>()->setRotation(0);
	//box.GetComponent<Rigidbody2D>()->setGravity(false);

	ground.setName("Ground");
	ground.AddComponent<BoxCollider2D>();
	ground.AddComponent<Box>();
	ground.GetComponent<Box>()->setSize(6, 0.5);
	ground.GetComponent<Box>()->fill();
	ground.GetComponent<Box>()->setColor(0, 0, 255, 255);
	ground.AddComponent<Rigidbody2D>();
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2), true);
	ground.GetComponent<BoxCollider2D>()->setSize(6, 0.5);
	ground.GetComponent<Rigidbody2D>()->setMass(400);
	ground.GetComponent<Rigidbody2D>()->setVelocity({ 0, 0 });
	ground.GetComponent<Rigidbody2D>()->setAngularSpeed(0);
	ground.GetComponent<Transform2D>()->setRotation(0);
	ground.GetComponent<Rigidbody2D>()->setGravity(false);

	std::vector<Entity*> entities = { &ground, &box };
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

	if (Collision2D::satDetection(box, ground)) {
		box.GetComponent<BoxCollider2D>()->update();
		ground.GetComponent<BoxCollider2D>()->update();
		CollisionInfo collisionInfo = Collision2D::getCollisionInfo(box, ground);

		Collision2D::resolveFullCollision(box, ground, collisionInfo);
	}

	//Collision2D::resolveCollisions();
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