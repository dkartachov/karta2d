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

	// int flags = IMG_INIT_PNG;
	// if (!(IMG_Init(flags) & flags)) {
	// 	printf("Image initialization error: %s\n", IMG_GetError());
	// 	return false;
	// }

	// if (TTF_Init() == -1) {
	// 	printf("TTF initilization error: %s\n", TTF_GetError());
	// 	return false;
	// }

	return true;
}

//SDL_Texture* Graphics::loadTexture(std::string path) {
//
//	SDL_Surface* surface = IMG_Load(path.c_str());
//	if (surface == NULL)
//		printf("Image load error: %s\n", IMG_GetError());
//
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//	if (texture == NULL)
//		printf("Create texture error: %s\n", SDL_GetError());
//
//	SDL_FreeSurface(surface);
//
//	return texture;
//}

// SDL_Texture* Graphics::createText(TTF_Font* font, std::string text, SDL_Color color) {

// 	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
// 	if (surface == NULL)
// 		printf("Text create error: %s", TTF_GetError());

// 	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
// 	if (texture == NULL)
// 		printf("Text render error: %s", SDL_GetError());

// 	SDL_FreeSurface(surface);

// 	return texture;
// }

void Graphics::drawBox(Transform2D transform, Vector2D size, bool fill, SDL_Color color) {

	// vertices calculation
	Vector2D position = transform.getPosition() * METERS_TO_PIXELS;
	double w = size.x, h = size.y;
	double rotation = transform.getRotation() * DEG_TO_RAD;
	double diagonal = sqrt(w * w + h * h);
	double diagonalAngle = atan2(h, w);

	Vector2D vertex1 = position - (diagonal / 2) * Vector2D(cos(diagonalAngle + rotation), sin(diagonalAngle + rotation));
	Vector2D vertex2 = position - (diagonal / 2) * Vector2D(-cos(diagonalAngle - rotation), sin(diagonalAngle - rotation));
	Vector2D vertex3 = position - (diagonal / 2) * Vector2D(-cos(diagonalAngle + rotation), -sin(diagonalAngle + rotation));
	Vector2D vertex4 = position - (diagonal / 2) * Vector2D(cos(diagonalAngle - rotation), -sin(diagonalAngle - rotation));

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_RenderDrawLineF(renderer, vertex1.x, vertex1.y, vertex2.x, vertex2.y); // vertex 1 to vertex 2
	SDL_RenderDrawLineF(renderer, vertex2.x, vertex2.y, vertex3.x, vertex3.y); // vertex 2 to vertex 3
	SDL_RenderDrawLineF(renderer, vertex3.x, vertex3.y, vertex4.x, vertex4.y); // vertex 3 to vertex 4
	SDL_RenderDrawLineF(renderer, vertex4.x, vertex4.y, vertex1.x, vertex1.y); // vertex 4 to vertex 1

	double xx1 = vertex1.x, yy1 = vertex1.y;
	double xx2 = vertex2.x, yy2 = vertex2.y;
	double d = 0.5 * sqrt(2);
	double dx = abs(d * sin(rotation));
	double dy = abs(d * cos(rotation));

	if (fill) {
		while (abs(xx1 - vertex4.x) >= d || abs(yy1 - vertex4.y) >= d) {
			xx1 = xx1 > vertex4.x ? xx1 - dx : (xx1 < vertex4.x ? xx1 + dx : xx1);
			yy1 = yy1 > vertex4.y ? yy1 - dy : (yy1 < vertex4.y ? yy1 + dy : yy1);
			xx2 = xx2 > vertex3.x ? xx2 - dx : (xx2 < vertex3.x ? xx2 + dx : xx2);
			yy2 = yy2 > vertex3.y ? yy2 - dy : (yy2 < vertex3.y ? yy2 + dy : yy2);

			SDL_RenderDrawLineF(renderer, xx1, yy1, xx2, yy2);
		}
	}

	SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
}

void Graphics::drawCircle(Vector2D position, int radius, int thickness, SDL_Color outlineColor, bool filled, SDL_Color fillColor) {
	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);

	int t = thickness <= radius ? radius - thickness : 0;
	bool done = false;

	do {
		float x = radius - 1;
		float y = 0;
		float dx = 1;
		float dy = 1;
		const float diameter = 2 * radius;
		float error = (dx - diameter);

		while (x >= y) {
			SDL_RenderDrawPointF(renderer, position.x + x, position.y + y);
			SDL_RenderDrawPointF(renderer, position.x + x, position.y - y);
			SDL_RenderDrawPointF(renderer, position.x - x, position.y + y);
			SDL_RenderDrawPointF(renderer, position.x - x, position.y - y);
			SDL_RenderDrawPointF(renderer, position.x + y, position.y + x);
			SDL_RenderDrawPointF(renderer, position.x + y, position.y - x);
			SDL_RenderDrawPointF(renderer, position.x - y, position.y + x);
			SDL_RenderDrawPointF(renderer, position.x - y, position.y - x);

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

		radius--;

		if (radius <= t) {
			if (filled && x > -1) {
				SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
			}
			else {
				done = true;
			}
		}

	} while (!done);
	
	SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, BACKGROUND.a);
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

	//TTF_Quit();
	//IMG_Quit();
	SDL_Quit();
}