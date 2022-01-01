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
	smolCircle.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 100, Graphics::Instance()->SCREEN_HEIGHT / 2 + 70));
	smolCircle.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(150, 0));
	smolCircle.GetComponent<CircleCollider2D>()->setRadius(25);
	smolCircle.GetComponent<Rigidbody2D>()->setMass(1);

	beegCircle.setName("Beeg Circle");
	beegCircle.AddComponent<CircleCollider2D>();
	beegCircle.AddComponent<Rigidbody2D>();
	beegCircle.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 100, Graphics::Instance()->SCREEN_HEIGHT / 2));
	beegCircle.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(-50, 0));
	beegCircle.GetComponent<Rigidbody2D>()->setMass(2);

	whiteBox.setName("White Box");
	whiteBox.AddComponent<BoxCollider2D>();
	whiteBox.AddComponent<Rigidbody2D>();
	whiteBox.GetComponent<Transform2D>()->setPosition({ 100, 200 });
	whiteBox.GetComponent<Rigidbody2D>()->setVelocity({ 20, 200 });

	redBox.setName("White Box");
	redBox.AddComponent<BoxCollider2D>();
	redBox.GetComponent<BoxCollider2D>()->setSize(100, 100);
	redBox.AddComponent<Rigidbody2D>();
	redBox.GetComponent<Transform2D>()->setPosition({ 100, 600});
	redBox.GetComponent<Rigidbody2D>()->setVelocity({ 0, -50 });
	redBox.GetComponent<Rigidbody2D>()->setMass(2);

	std::vector<Entity*> entities = { &smolCircle, &beegCircle, &whiteBox, &redBox };
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