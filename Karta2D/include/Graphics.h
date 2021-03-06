#pragma once
#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL_ttf.h>
#include <string>
#include "Vector2D.h"
#include "Transform2D.h"

class Graphics {
public:
	static Graphics* Instance();
	static void release();

	Graphics();
	bool init();

	SDL_Texture* loadTexture(std::string path);
	//SDL_Texture* createText(TTF_Font* font, std::string text, SDL_Color color);

	void drawBox(Transform2D transform, Vector2D size, bool fill = false, SDL_Color color = { 0, 0, 0, 255 });
	void drawCircle(Vector2D position, int radius, int thickness, SDL_Color outlineColor = { 255, 255, 255, 255 }, bool filled = false, SDL_Color fillColor = { 255, 255, 255, 255 });
	void drawTexture(SDL_Texture* texture, SDL_Rect* clip = NULL, SDL_Rect* rect = NULL, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void clear();
	void render();
	void exit();
public:
	static const int SCREEN_WIDTH = 1400;
	static const int SCREEN_HEIGHT = 1000;
private:
	const char* TITLE = "Karta2D";
	const SDL_Color BACKGROUND = { 128, 128, 128, 255 };
	static Graphics* instance;
	SDL_Window* window;
public:
	SDL_Renderer* renderer;
};