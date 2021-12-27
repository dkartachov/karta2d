#include <iostream>
#include "Application.h"

int main(int argc, char** args) {

	Application* app = Application::Instance();

	app->run();
	app->exit();

	//std::cin.get();
	return 0;
}