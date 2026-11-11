#include <cstdint>
#include <iostream>
#include <limits>
#include <print>
#include <vector>

// Вариант 1

struct halfedge {
  uint64_t vertex;
  int64_t weight;
  explicit halfedge(uint64_t _vertex = 0, uint64_t _weight = 0)
      : vertex(_vertex), weight(_weight) {};
  bool operator<(const halfedge &other) const {
    return this->weight < other.weight;
  }
  bool operator>(const halfedge &other) const {
    return this->weight > other.weight;
  }
  bool operator<=(const halfedge &other) const {
    return this->weight <= other.weight;
  }
  bool operator>=(const halfedge &other) const {
    return this->weight >= other.weight;
  }
};

struct edge {
  uint64_t from;
  uint64_t to;
  int64_t weight;
  explicit edge(uint64_t _from = 0, uint64_t _to = 0, uint64_t _weight = 0)
      : from(_from), to(_to), weight(_weight) {};
  edge(halfedge _half, uint64_t _to)
      : from(_half.vertex), to(_to), weight(_half.weight) {};
  edge(uint64_t _from, halfedge _half)
      : from(_from), to(_half.vertex), weight(_half.weight) {};
  bool operator<(const edge &other) const {
    return this->weight < other.weight;
  }
  bool operator>(const edge &other) const {
    return this->weight > other.weight;
  }
  bool operator<=(const edge &other) const {
    return this->weight <= other.weight;
  }
  bool operator>=(const edge &other) const {
    return this->weight >= other.weight;
  }
};

using graph_t = std::vector<std::vector<halfedge>>;

uint64_t get_degree(const graph_t &graph, const uint64_t vertex) {
  uint64_t degree = 0;
  degree += graph[vertex].size();
  return degree;
}

uint64_t get_double_degree(const graph_t &graph, const uint64_t vertex) {
  uint64_t double_degree = 0;
  for (const auto &current_edge : graph[vertex]) {
    double_degree += get_degree(graph, current_edge.vertex);
  }
  return double_degree;
}

int main() {
  uint64_t vertex_count;
  uint64_t edge_count;
  std::cin >> vertex_count >> edge_count;
  graph_t graph(vertex_count);
  for (uint64_t i = 0; i < edge_count; i++) {
    uint64_t from;
    uint64_t to;
    std::cin >> from >> to;
    graph[from].push_back(halfedge(to));
    graph[to].push_back(halfedge(from));
  }
  std::println("Двойная степень вершины {}:{}", 0, get_double_degree(graph, 0));
  return 0;
}
