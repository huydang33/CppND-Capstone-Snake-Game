#include "snake.h"
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <algorithm>
#include <vector>
#include <memory>

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

  return neighbors;
}

bool isValid(int x, int y, const std::vector<std::vector<int>>& grid) {
    return x >= 0 && y >= 0 && x < grid.size() && y < grid[0].size() && grid[x][y] == 0;
}

std::vector<Node> SmartSnake::FindPath(SDL_Point food) 
{
  std::priority_queue<Node, std::vector<Node>, CompareNodes> open_set;
  std::unordered_set<std::string> closed_set;

  /* Push start point to open list*/
  SDL_Point startPoint = {static_cast<int>(head_x), static_cast<int>(head_y)};
  open_set.push(Node(&startPoint, 0, Heuristic(startPoint, food)));

  while (!open_set.empty()) {
    // Lowest cost node from open_set
    Node current = open_set.top();
    open_set.pop();

    // Check if reach the food
    if (current.point->x == food.x && current.point->y == food.y) {
        std::vector<Node> path;
        Node* temp = &current;

        // Build the path
        while (temp) {
            path.push_back(*temp);
            temp = temp->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Add to closed set
    closed_set.insert(std::to_string(current.point->x) + "," + std::to_string(current.point->y));

    // Get neighbors node
    for (const auto& neighbor_pos : GetNeighbors(*current.point)) 
    {
      if (SnakeCell(neighbor_pos.x, neighbor_pos.y)) continue; // Skip if it in Snake cell

      std::string neighbor_key = std::to_string(neighbor_pos.x) + "," + std::to_string(neighbor_pos.y);
      if (closed_set.find(neighbor_key) != closed_set.end()) continue;

      // Calculate g(x) and h(x)
      int g_cost = current.gCost + 1;
      int h_cost = Heuristic(neighbor_pos, food);
      auto neighbor_point = std::make_unique<SDL_Point>(neighbor_pos);
      Node* neighborNode = new Node(neighbor_point.release(), g_cost, h_cost, &current);

      // Add to Open Set
      open_set.push(*neighborNode);
    }
  }

  return {};
}