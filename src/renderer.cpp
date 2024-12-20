
#include "game.h"
#include "renderer.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "util.h"

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      sdl_window(nullptr),
      sdl_renderer(nullptr) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    return; // Exit constructor early if SDL_Init fails
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_Quit(); // Clean up SDL if window creation fails
    return;
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(sdl_window); // Clean up window if renderer creation fails
    SDL_Quit();
    return;
  }
}

Renderer::~Renderer() {
  if (sdl_renderer) {
    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = nullptr;
  }
  if (sdl_window) {
    SDL_DestroyWindow(sdl_window);
    sdl_window = nullptr;
  }
  SDL_Quit();
}

void Renderer::InitObstacle(int num_obstacles, GAME_DIFFICULTY_LEVEL diff_level)
{
  std::srand(std::time(0));

  // Clear any existing obstacles
  obstacles.clear();

  // Generate random obstacles
  for (int i = 0; i < num_obstacles; ++i) {
    std::vector<SDL_Point> wall;
    int wall_length = diff_level == DIFF_NORMAL ? 2 : std::rand() % 3 + 2;  // Random wall length from 2 to 4

    // Random starting point
    SDL_Point start_point;
    start_point.x = std::rand() % grid_width;
    start_point.y = std::rand() % grid_height;
    wall.push_back(start_point);

    // Randomize direction: 0 = horizontal, 1 = vertical, 2 = diagonal
    int direction = std::rand() % 3;

    for (int j = 1; j < wall_length; ++j) {
      SDL_Point next_point = wall.back(); // Get the last point in the wall

      switch (direction) {
        case 0: // Horizontal
          next_point.x = (next_point.x + 1) % grid_width;
          break;
        case 1: // Vertical
          next_point.y = (next_point.y + 1) % grid_height;
          break;
        case 2: // Diagonal
          next_point.x = (next_point.x + 1) % grid_width;
          next_point.y = (next_point.y + 1) % grid_height;
          break;
      }

      wall.push_back(next_point);
    }

    obstacles.push_back(wall);
  }
}

void Renderer::Render(Snake const snake, SDL_Point const &food, bool special_food_active, SDL_Point special_food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render special food
  if(special_food_active)
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
    block.x = special_food.x * block.w;
    block.y = special_food.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render obstacles
  SDL_SetRenderDrawColor(sdl_renderer, 0xA5, 0x2A, 0x2A, 0xFF);
  for (const auto &obstacle : obstacles) {
    for (SDL_Point const &point : obstacle) {
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update screen after rendering all components
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
