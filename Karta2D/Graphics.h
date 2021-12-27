#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "Tools/Vector2D.h"

class Graphics {
public:
	static Graphics* Instance();
	static void release();

	Graphics();
	bool init();

	SDL_Texture* loadTexture(std::string path);
	SDL_Texture* createText(TTF_Font* font, std::string text, SDL_Color color);

	void drawSquare(SDL_Rect* rect, SDL_Color color = { 255, 255, 255, 255 });
	void drawCircle(Vector2D position, const int radius, bool fill, SDL_Color color = { 255, 255, 255, 255 });
	void drawTexture(SDL_Texture* texture, SDL_Rect* clip = NULL, SDL_Rect* rect = NULL, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void clear();
	void render();
	void exit();
public:
	static const int SCREEN_WIDTH = 1600;
	static const int SCREEN_HEIGHT = 1200;
private:
	const char* TITLE = "Application";
	static Graphics* instance;
	SDL_Window* window;
	SDL_Renderer* renderer;
};