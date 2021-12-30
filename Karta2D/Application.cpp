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

	parentBox.setName("Parent");
	parentBox.AddComponent<Box>();
	parentBox.AddComponent<BoxCollider2D>();
	parentBox.GetComponent<BoxCollider2D>()->setSize(100, 100);
	parentBox.GetComponent<Box>()->setSize(100, 100);
	parentBox.GetComponent<Box>()->fill();
	parentBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2));

	childBox.setName("Child");
	childBox.AddComponent<Box>();
	childBox.AddComponent<BoxCollider2D>();
	childBox.GetComponent<BoxCollider2D>()->setSize(50, 50);
	childBox.GetComponent<Box>()->setSize(50, 50);
	childBox.GetComponent<Box>()->setColor(0, 0, 0, 255);
	childBox.GetComponent<Box>()->fill();
	childBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 200, Graphics::Instance()->SCREEN_HEIGHT / 2));

	childerBox.setName("Childer");
	childerBox.AddComponent<Box>();
	childerBox.AddComponent<BoxCollider2D>();
	childerBox.GetComponent<BoxCollider2D>()->setSize(50, 50);
	childerBox.GetComponent<Box>()->setSize(50, 50);
	childerBox.GetComponent<Box>()->setColor(0, 0, 0, 255);
	childerBox.GetComponent<Box>()->fill();
	childerBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 200, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200));

	std::vector<Entity*> entities = { &parentBox, &childBox, &childerBox };
	EntityManager::getInstance().addEntities(entities);

	std::printf("%d\n", (int)EntityManager::getInstance().getEntities().size());
}

Application::~Application() {
	Graphics::release();
	graphics = nullptr;
}

void Application::earlyUpdate() {
	timer->reset();
}

void Application::update() {
	//parentBox.GetComponent<Transform2D>()->rotate(50 * timer->Instance()->getDeltaTime());
	parentBox.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(-30, 0));
	childerBox.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(0, 30));

	EntityManager::getInstance().update();

	//parentBox.update();
	//childBox.update();
}

void Application::lateUpdate() {
	//std::printf("%s\n", Collision2D::AABB(parentBox, childBox) ? "Collision!" : "No Collision.");
	Collision2D::resolveAABBCollisions();
}

void Application::render() {

	graphics->clear();

	////RENDER ENTITIES HERE////

	EntityManager::getInstance().render();
	//parentBox.render();
	//childBox.render();

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
					parentBox.GetComponent<Transform2D>()->translate(Vector2D(0, -50));
					break;
				case SDLK_s:
					parentBox.GetComponent<Transform2D>()->translate(Vector2D(0, 50));
					break;
				case SDLK_a:
					parentBox.GetComponent<Transform2D>()->translate(Vector2D(-50, 0));
					break;
				case SDLK_d:
					parentBox.GetComponent<Transform2D>()->translate(Vector2D(50, 0));
					break;
				case SDLK_q:
					parentBox.GetComponent<Transform2D>()->setRotation(20);
					break;
				case SDLK_e:
					parentBox.GetComponent<Transform2D>()->setRotation(-20);
					break;
				case SDLK_v:
					parentBox.GetComponent<BoxCollider2D>()->setVisibility(!parentBox.GetComponent<BoxCollider2D>()->isVisible());
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