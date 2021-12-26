#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class Graphics {
public:
	static Graphics* Instance();
	static void release();

	Graphics();
	bool init();

	SDL_Texture* loadTexture(std::string path);
	SDL_Texture* createText(TTF_Font* font, std::string text, SDL_Color color);

	void drawTexture(SDL_Texture* texture, SDL_Rect* clip = NULL, SDL_Rect* rect = NULL, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void clear();
	void render();
	void exit();
public:
	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 800;
private:
	const char* TITLE = "Application";
	static Graphics* instance;
	SDL_Window* window;
	SDL_Renderer* renderer;
};