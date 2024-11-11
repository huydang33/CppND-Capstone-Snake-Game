#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>
#include "SDL.h"

struct Node {
  SDL_Point *point;              // Current position (x,y)
  int gCost;                    // Cost from the start node to current node
  int hCost;                    // Cost from current node to finish node
  Node* parent;                 // Pointer to the previous node in the path

  Node(SDL_Point *p, int g, int h, Node* prt = nullptr)
      : point(p), gCost(g), hCost(h), parent((prt)) {}
};

struct CompareNodes {
  bool operator()(Node a, Node b) const {
      // Trả về true nếu a có chi phí lớn hơn b, vì priority_queue ưu tiên phần tử lớn nhất
      return (a.gCost + a.hCost) > (b.gCost + b.hCost);
  }
};

struct PointHash {
  std::size_t operator()(const std::pair<int, int>& point) const {
      // Sử dụng công thức băm cơ bản cho hai số nguyên
      return std::hash<int>()(point.first) ^ (std::hash<int>()(point.second) << 1);
  }
};

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

class SmartSnake: public Snake {
public:
  SmartSnake(int grid_width, int grid_height): Snake(grid_width, grid_height) {}

  std::vector<SDL_Point> FindPath(SDL_Point food);

private:
  std::vector<SDL_Point> GetNeighbors(const SDL_Point &node);
  int Heuristic(const SDL_Point &a, const SDL_Point &b);
};

#endif