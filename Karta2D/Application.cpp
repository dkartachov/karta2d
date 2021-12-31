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

	smolCircle.setName("Smol Circle");
	smolCircle.AddComponent<CircleCollider2D>();
	smolCircle.AddComponent<Rigidbody2D>();
	smolCircle.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 100, Graphics::Instance()->SCREEN_HEIGHT / 2 + 50));
	smolCircle.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(50, 0));

	beegCircle.setName("Beeg Circle");
	beegCircle.AddComponent<CircleCollider2D>();
	beegCircle.AddComponent<Rigidbody2D>();
	beegCircle.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 100, Graphics::Instance()->SCREEN_HEIGHT / 2));
	beegCircle.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(-50, 0));

	std::vector<Entity*> entities = { &smolCircle, &beegCircle};
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
	//Collision2D::resolveAABBCollisions();
	Collision2D::CircleCircle(beegCircle, smolCircle);
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