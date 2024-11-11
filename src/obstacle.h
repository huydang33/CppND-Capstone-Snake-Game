#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>
#include <iostream>
#include <memory>

// Abstract Obstacle
class Obstacle {
public:
    Obstacle(int x, int y);
    virtual ~Obstacle() = default;

    virtual void Move() = 0;
    virtual void DisplayPosition() const;
    virtual void Render(SDL_Renderer* renderer) = 0;

protected:
    int posX, posY;
};

class Rock : public Obstacle {
public:
    Rock(int x, int y, SDL_Texture* texture);
    ~Rock();

    void Move() override;
    void DisplayPosition() const override;
    void Render(SDL_Renderer* renderer) override;
private:
    SDL_Texture* texture_;
};

// Bird
class Bird : public Obstacle {
public:
    Bird(int x, int y, SDL_Texture* texture);
    ~Bird();

    void Move() override;
    void DisplayPosition() const override;
    void Render(SDL_Renderer* renderer) override;
private:
    SDL_Texture* texture_;
};

#endif // OBSTACLE_H