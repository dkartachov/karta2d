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

	box1.setName("Box 1");
	box1.AddComponent<BoxCollider2D>();
	box1.AddComponent<Box>();
	box1.GetComponent<Box>()->setSize(1.5, 1);
	box1.GetComponent<Box>()->fill();
	box1.GetComponent<Box>()->setColor(255, 0, 0, 255);
	box1.AddComponent<Rigidbody2D>();
	box1.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 20), true);
	box1.GetComponent<BoxCollider2D>()->setSize(1.5, 1);
	box1.GetComponent<Rigidbody2D>()->setMass(2);
	box1.GetComponent<Rigidbody2D>()->setVelocity({ 0, 0 });
	//box1.GetComponent<Rigidbody2D>()->setGravity(false);

	box2.setName("Box 2");
	box2.AddComponent<BoxCollider2D>();
	box2.AddComponent<Box>();
	box2.GetComponent<Box>()->setSize(0.5, 0.5);
	box2.GetComponent<Box>()->fill();
	box2.GetComponent<Box>()->setColor(0, 255, 0, 255);
	box2.AddComponent<Rigidbody2D>();
	box2.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 600, Graphics::Instance()->SCREEN_HEIGHT / 2 + 200), true);
	box2.GetComponent<BoxCollider2D>()->setSize(0.5, 0.5);
	box2.GetComponent<Rigidbody2D>()->setMass(1);
	box2.GetComponent<Rigidbody2D>()->setVelocity({ 3, -8 });
	//box3.GetComponent<Rigidbody2D>()->setAngularSpeed(-360);
	//box3.GetComponent<Rigidbody2D>()->setGravity(false);

	ground.setName("Ground");
	ground.AddComponent<BoxCollider2D>();
	ground.AddComponent<Box>();
	ground.GetComponent<Box>()->setSize(6, 0.5);
	ground.GetComponent<Box>()->fill();
	ground.GetComponent<Box>()->setColor(0, 0, 255, 255);
	ground.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100), true);
	ground.GetComponent<BoxCollider2D>()->setSize(6, 0.5);

	std::vector<Entity*> entities = { &box1, &box2, &ground };
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