#include "obstacle.h"

Obstacle::Obstacle(int x, int y) : posX(x), posY(y) {}

void Obstacle::DisplayPosition() const {
    std::cout << "Obstacle at (" << posX << ", " << posY << ")" << std::endl;
}

Rock::Rock(int x, int y, SDL_Texture* texture) : Obstacle(x, y), texture_(texture) {}
Rock::~Rock() {}

void Rock::Move() {}

void Rock::DisplayPosition() const {
    std::cout << "Rock at (" << posX << ", " << posY << ")" << std::endl;
}

void Rock::Render(SDL_Renderer* renderer) {
    SDL_Rect destRect = { posX, posY, 32, 32 }; // 32x32 pixel
    SDL_RenderCopy(renderer, texture_, nullptr, &destRect);
}

Bird::Bird(int x, int y, SDL_Texture* texture) : Obstacle(x, y), texture_(texture) {}
Bird::~Bird() {}

void Bird::Move() {
    posX += 5;
}

void Bird::DisplayPosition() const {
    std::cout << "Bird at (" << posX << ", " << posY << ")" << std::endl;
}

void Bird::Render(SDL_Renderer* renderer) {
    SDL_Rect destRect = { posX, posY, 32, 32 };
    SDL_RenderCopy(renderer, texture_, nullptr, &destRect);
}
