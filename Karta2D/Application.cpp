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
	parentBox.GetComponent<BoxCollider2D>()->setSize(50, 50);
	parentBox.GetComponent<Box>()->setSize(50, 50);
	parentBox.GetComponent<Box>()->fill();
	parentBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2));

	childBox.setName("Child");
	childBox.AddComponent<Box>();
	childBox.AddComponent<BoxCollider2D>();
	childBox.GetComponent<BoxCollider2D>()->setSize(50, 600);
	childBox.GetComponent<Box>()->setSize(50, 600);
	childBox.GetComponent<Box>()->setColor(0, 0, 0, 255);
	childBox.GetComponent<Box>()->fill();
	childBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 400, Graphics::Instance()->SCREEN_HEIGHT / 2));

	std::vector<Entity*> entities = { &parentBox, &childBox };
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
	parentBox.GetComponent<Transform2D>()->translate(timer->Instance()->getDeltaTime() * Vector2D(-200, 0));

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