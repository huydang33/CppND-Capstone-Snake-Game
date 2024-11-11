#include "snake.h"
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <queue>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

int SmartSnake::Heuristic(const SDL_Point &a, const SDL_Point &b) {
  // Manhattan distance
  return abs(a.x - b.x) + abs(a.y - b.y);
}

std::vector<SDL_Point> SmartSnake::GetNeighbors(const SDL_Point &node)
{
  std::vector<SDL_Point> neighbors;

  /* Up, down, left, right */
  neighbors.push_back({node.x + 1, node.y});
  neighbors.push_back({node.x - 1, node.y});
  neighbors.push_back({node.x, node.y + 1});
  neighbors.push_back({node.x, node.y - 1});
}

std::vector<SDL_Point> SmartSnake::FindPath(SDL_Point food) {
  std::priority_queue<Node, std::vector<Node>, CompareNodes> openList;
  std::unordered_set<SDL_Point, PointHash> closedList;

  /* Push start point to open list*/
  SDL_Point startPoint = {static_cast<int>(head_x), static_cast<int>(head_y)};
  openList.push(Node(&startPoint, 0, Heuristic(startPoint, food)));



  return {};
}