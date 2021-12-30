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

	whiteBox.setName("White Box");
	whiteBox.AddComponent<Box>();
	whiteBox.AddComponent<BoxCollider2D>();
	whiteBox.AddComponent<Rigidbody2D>();
	whiteBox.GetComponent<BoxCollider2D>()->setSize(30, 30);
	whiteBox.GetComponent<Box>()->setSize(30, 30);
	whiteBox.GetComponent<Box>()->fill();
	whiteBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 100));
	whiteBox.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(-200, -200));
	whiteBox.GetComponent<Rigidbody2D>()->setMass(5);

	redBox.setName("Red Box");
	redBox.AddComponent<Box>();
	redBox.AddComponent<BoxCollider2D>();
	redBox.AddComponent<Rigidbody2D>();
	redBox.GetComponent<BoxCollider2D>()->setSize(30, 30);
	redBox.GetComponent<Box>()->setSize(30, 30);
	redBox.GetComponent<Box>()->setColor(255, 0, 0, 255);
	redBox.GetComponent<Box>()->fill();
	redBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 - 100));
	redBox.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(200, -200));
	redBox.GetComponent<Rigidbody2D>()->setMass(5);

	blueBox.setName("Blue Box");
	blueBox.AddComponent<Box>();
	blueBox.AddComponent<BoxCollider2D>();
	blueBox.AddComponent<Rigidbody2D>();
	blueBox.GetComponent<BoxCollider2D>()->setSize(30, 30);
	blueBox.GetComponent<Box>()->setSize(30, 30);
	blueBox.GetComponent<Box>()->setColor(0, 0, 255, 255);
	blueBox.GetComponent<Box>()->fill();
	blueBox.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 50, Graphics::Instance()->SCREEN_HEIGHT / 2));
	blueBox.GetComponent<Rigidbody2D>()->setVelocity(Vector2D(100, 200));
	blueBox.GetComponent<Rigidbody2D>()->setMass(5);

	leftWall.setName("Left Wall");
	leftWall.AddComponent<Box>();
	leftWall.AddComponent<BoxCollider2D>();
	leftWall.GetComponent<BoxCollider2D>()->setSize(50, 800);
	leftWall.GetComponent<Box>()->setSize(50, 800);
	leftWall.GetComponent<Box>()->setColor(0, 0, 0, 255);
	leftWall.GetComponent<Box>()->fill();
	leftWall.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 - 200, Graphics::Instance()->SCREEN_HEIGHT / 2));

	rightWall.setName("Right Wall");
	rightWall.AddComponent<Box>();
	rightWall.AddComponent<BoxCollider2D>();
	rightWall.GetComponent<BoxCollider2D>()->setSize(50, 800);
	rightWall.GetComponent<Box>()->setSize(50, 800);
	rightWall.GetComponent<Box>()->setColor(0, 0, 0, 255);
	rightWall.GetComponent<Box>()->fill();
	rightWall.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2 + 200, Graphics::Instance()->SCREEN_HEIGHT / 2));

	ceiling.setName("Ceiling");
	ceiling.AddComponent<Box>();
	ceiling.AddComponent<BoxCollider2D>();
	ceiling.GetComponent<BoxCollider2D>()->setSize(800, 50);
	ceiling.GetComponent<Box>()->setSize(800, 50);
	ceiling.GetComponent<Box>()->setColor(0, 0, 0, 255);
	ceiling.GetComponent<Box>()->fill();
	ceiling.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 - 200));

	floor.setName("Floor");
	floor.AddComponent<Box>();
	floor.AddComponent<BoxCollider2D>();
	floor.GetComponent<BoxCollider2D>()->setSize(800, 50);
	floor.GetComponent<Box>()->setSize(800, 50);
	floor.GetComponent<Box>()->setColor(0, 0, 0, 255);
	floor.GetComponent<Box>()->fill();
	floor.GetComponent<Transform2D>()->setPosition(Vector2D(Graphics::Instance()->SCREEN_WIDTH / 2, Graphics::Instance()->SCREEN_HEIGHT / 2 + 200));

	std::vector<Entity*> entities = { &whiteBox, &redBox, &blueBox, &leftWall, &rightWall, &ceiling, &floor };
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