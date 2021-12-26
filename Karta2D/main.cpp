#include <iostream>
#include <SDL.h>
#include "SimpleECS/SimpleECS.h"
#include "SimpleECS/Components.h"

int main(int argc, char** args) {

	Entity ent;

	ent.AddComponent<Transform2D>();
	ent.GetComponent<Transform2D>()->toString();

	std::cin.get();
	return 0;
}