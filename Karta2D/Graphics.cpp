#include "Graphics.h"

Graphics* Graphics::instance = NULL;

Graphics* Graphics::Instance() {
	if (instance == NULL)
		instance = new Graphics();

	return instance;
}

void Graphics::release() {
	delete instance;
	instance = NULL;
}

Graphics::Graphics() {
	init();
}

bool Graphics::init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Cannot create window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Cannot create renderer: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags)) {
		printf("Image initialization error: %s\n", IMG_GetError());
		return false;
	}

	if (TTF_Init() == -1) {
		printf("TTF initilization error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

SDL_Texture* Graphics::loadTexture(std::string path) {

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
		printf("Image load error: %s\n", IMG_GetError());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
		printf("Create texture error: %s\n", SDL_GetError());

	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* Graphics::createText(TTF_Font* font, std::string text, SDL_Color color) {

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (surface == NULL)
		printf("Text create error: %s", TTF_GetError());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
		printf("Text render error: %s", SDL_GetError());

	SDL_FreeSurface(surface);

	return texture;
}

void Graphics::drawSquare(SDL_Rect* rect, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Graphics::drawCircle(Vector2D position, const int radius, bool fill, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	const int diameter = 2 * radius;
	int error = (dx - diameter);

	while (x >= y) {
		SDL_RenderDrawPoint(renderer, position.x + x, position.y + y);
		SDL_RenderDrawPoint(renderer, position.x + x, position.y - y);
		SDL_RenderDrawPoint(renderer, position.x - x, position.y + y);
		SDL_RenderDrawPoint(renderer, position.x - x, position.y - y);
		SDL_RenderDrawPoint(renderer, position.x + y, position.y + x);
		SDL_RenderDrawPoint(renderer, position.x + y, position.y - x);
		SDL_RenderDrawPoint(renderer, position.x - y, position.y + x);
		SDL_RenderDrawPoint(renderer, position.x - y, position.y - x);

		if (error <= 0) {
			y++;
			error += dy;
			dy += 2;
		}

		if (error > 0) {
			x--;
			dx += 2;
			error += (dx - diameter);
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Graphics::drawTexture(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* rect, float angle, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, clip, rect, angle, NULL, flip);
}

void Graphics::clear() {
	SDL_RenderClear(renderer);
}

void Graphics::render() {
	SDL_RenderPresent(renderer);
}

void Graphics::exit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}