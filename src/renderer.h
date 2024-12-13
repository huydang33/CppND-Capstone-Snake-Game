#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "util.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();
  void InitObstacle(int num_obstacles, GAME_DIFFICULTY_LEVEL diff_level);
  void Render(Snake const snake, SDL_Point const &food, bool special_food_active, SDL_Point special_food);
  void UpdateWindowTitle(int score, int fps);
  std::vector<std::vector<SDL_Point>> obstacles;

  std::size_t GetGridWidth() { return grid_width; };
  std::size_t GetGridHeight() { return grid_height; };


 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif