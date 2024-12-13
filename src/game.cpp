#include "game.h"
#include <iostream>
#include "SDL.h"
#include "util.h"
#include <thread>
#include <mutex>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration, GAME_DIFFICULTY_LEVEL diff_level) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  int num_obstacles = 3;
  bool running = true;
  this->diff_level = diff_level;

  // Initialize Obstacles
  if (diff_level > DIFF_EASY)
  {
    renderer.InitObstacle(num_obstacles, diff_level);
    this->obstacles = renderer.obstacles;
    snake.speed = diff_level == DIFF_NORMAL ? 0.15 : 0.25;
  }

  // Thread synchronization for special food
  std::mutex food_mutex;
  bool special_food_active = false;
  SDL_Point special_food;
  Uint32 special_food_timer = 0;

  // Thread to handle special food logic
  std::thread special_food_thread([&]() 
  {
    while (running) 
    {
      {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for 5 seconds

        std::lock_guard<std::mutex> lock(food_mutex);
        if (!special_food_active) {
          // Generate random position for special food
          special_food.x = rand() % renderer.GetGridWidth();
          special_food.y = rand() % renderer.GetGridHeight();
          special_food_active = true;
          special_food_timer = SDL_GetTicks();
        }
      }

      // Wait for 5 seconds of active special food
      std::this_thread::sleep_for(std::chrono::seconds(5));

      {
        std::lock_guard<std::mutex> lock(food_mutex);
        if (special_food_active) {
          special_food_active = false;
        }
      }
    }
  });

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update(special_food_active);
    if (!snake.alive)
    {
      std::cout << "Snake died. End game!!\n";
      running = false;
      break;
    }
    // Render game objects
    {
      std::lock_guard<std::mutex> lock(food_mutex);
      renderer.Render(snake, food, special_food_active, special_food);

      // Check if snake eats special food
      if (special_food_active && snake.head_x == special_food.x && snake.head_y == special_food.y) {
        score += 2; // Double score for special food
        special_food_active = false;
      }
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  special_food_thread.join();
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (snake.SnakeCell(x, y)) {
      continue;
    }

    bool overlaps_with_obstacle = false;
    for(const auto &wall: this->obstacles) {
      for (SDL_Point const &obstacle : wall) {
        if (obstacle.x == x && obstacle.y == y) {
          overlaps_with_obstacle = true;
          break;
        }
      }
    }

    // Place food if not collapse with obstacles
    if (!overlaps_with_obstacle) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update(bool special_food_active) {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    switch(this->diff_level)
    {
      case DIFF_EASY:
        score += special_food_active ? 1 : 2;
        break;
      case DIFF_NORMAL:
        score += special_food_active ? 2 : 4;
        break;
      case DIFF_HARD:
        score += special_food_active ? 3 : 6;
        break;
    }
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  for(const auto &wall: this->obstacles) {
    for (SDL_Point const &obstacle : wall) {
      if (obstacle.x == new_x && obstacle.y == new_y) {
        snake.alive = false; // Snake die if hit obstacles
        return;
      }
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }