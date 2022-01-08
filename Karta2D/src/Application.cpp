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

	boxA.setName("red box");
	boxA.AddComponent<BoxCollider2D>();
	boxA.AddComponent<Box>();
	boxA.GetComponent<Box>()->setSize(60, 60);
	boxA.GetComponent<Box>()->fill();
	boxA.GetComponent<Box>()->setColor(255, 0, 0, 255);
	boxA.AddComponent<Rigidbody2D>();
	boxA.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2));
	boxA.GetComponent<BoxCollider2D>()->setSize(60, 60);
	boxA.GetComponent<Rigidbody2D>()->setMass(1);
	boxA.GetComponent<Rigidbody2D>()->setVelocity({ 0, 0 });
	//boxA.GetComponent<Rigidbody2D>()->setAngularSpeed(360);
	//boxA.GetComponent<Transform2D>()->setRotation(45);
	//boxA.GetComponent<Rigidbody2D>()->setGravity(false);

	boxB.setName("blue box");
	boxB.AddComponent<BoxCollider2D>();
	boxB.AddComponent<Box>();
	boxB.GetComponent<Box>()->setSize(60, 60);
	boxB.GetComponent<Box>()->fill();
	boxB.GetComponent<Box>()->setColor(0, 0, 255, 255);
	boxB.AddComponent<Rigidbody2D>();
	boxB.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 100, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	boxB.GetComponent<BoxCollider2D>()->setSize(60, 60);
	boxB.GetComponent<Rigidbody2D>()->setMass(1);
	boxB.GetComponent<Rigidbody2D>()->setVelocity({ -100, -100 });
	//boxB.GetComponent<Rigidbody2D>()->setAngularSpeed(0);
	//boxB.GetComponent<Rigidbody2D>()->setGravity(false);

	ground.setName("Ground");
	ground.AddComponent<BoxCollider2D>();
	ground.AddComponent<Box>();
	ground.GetComponent<Box>()->setSize(1600, 50);
	ground.GetComponent<Box>()->fill();
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 300));
	ground.GetComponent<BoxCollider2D>()->setSize(1600, 50);
	ground.GetComponent<BoxCollider2D>()->restitution = 0;

	wall.setName("Wall");
	wall.AddComponent<BoxCollider2D>();
	wall.AddComponent<Box>();
	wall.GetComponent<Box>()->setSize(50, 1600);
	wall.GetComponent<Box>()->fill();
	wall.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 600, Graphics::Instance()->SCREEN_HEIGHT / 2));
	wall.GetComponent<BoxCollider2D>()->setSize(50, 1600);

	std::vector<Entity*> entities = { &boxB, &ground, &boxA, &wall };
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