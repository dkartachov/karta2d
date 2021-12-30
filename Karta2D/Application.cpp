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

	whiteBox.setName("White");
	whiteBox.AddComponent<Box>();
	whiteBox.AddComponent<BoxCollider2D>();
	whiteBox.AddComponent<Rigidbody2D>();
	whiteBox.GetComponent<BoxCollider2D>()->setSize(50, 50);
	whiteBox.GetComponent<Box>()->setSize(50, 50);
	whiteBox.GetComponent<Box>()->fill();
	whiteBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 200));
	whiteBox.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(-100, -100));
	whiteBox.GetComponent<Rigidbody2D>()->setMass(10);

	redBox.setName("Red");
	redBox.AddComponent<Box>();
	redBox.AddComponent<BoxCollider2D>();
	redBox.AddComponent<Rigidbody2D>();
	redBox.GetComponent<BoxCollider2D>()->setSize(50, 50);
	redBox.GetComponent<Box>()->setSize(50, 50);
	redBox.GetComponent<Box>()->setColor(255, 0, 0, 255);
	redBox.GetComponent<Box>()->fill();
	redBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200));
	redBox.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(0, 20));
	redBox.GetComponent<Rigidbody2D>()->setMass(5);

	blackBox.setName("Black");
	blackBox.AddComponent<Box>();
	blackBox.AddComponent<BoxCollider2D>();
	blackBox.AddComponent<Rigidbody2D>();
	blackBox.GetComponent<BoxCollider2D>()->setSize(100, 100);
	blackBox.GetComponent<Box>()->setSize(100, 100);
	blackBox.GetComponent<Box>()->setColor(0, 0, 0, 255);
	blackBox.GetComponent<Box>()->fill();
	blackBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 400, Graphics::Instance()->SCREEN_HEIGHT / 2));
	blackBox.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(50, 0));
	blackBox.GetComponent<Rigidbody2D>()->setMass(30);

	std::vector<Entity*> entities = { &whiteBox, &blackBox, &redBox };
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
	EntityManager::getInstance().update();
}

void Application::lateUpdate() {
	Collision2D::resolveAABBCollisions();
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
				case SDLK_w:
					whiteBox.GetComponent<Transform2D>()->translate(Vector2D(0, -10));
					break;
				case SDLK_s:
					whiteBox.GetComponent<Transform2D>()->translate(Vector2D(0, 10));
					break;
				case SDLK_a:
					whiteBox.GetComponent<Transform2D>()->translate(Vector2D(-10, 0));
					break;
				case SDLK_d:
					whiteBox.GetComponent<Transform2D>()->translate(Vector2D(10, 0));
					break;
				case SDLK_q:
					whiteBox.GetComponent<Transform2D>()->setRotation(20);
					break;
				case SDLK_e:
					whiteBox.GetComponent<Transform2D>()->setRotation(-20);
					break;
				case SDLK_v:
					whiteBox.GetComponent<BoxCollider2D>()->setVisibility(!whiteBox.GetComponent<BoxCollider2D>()->isVisible());
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