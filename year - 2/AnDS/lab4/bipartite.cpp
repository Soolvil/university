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

bool is_bipartite(const graph_t &graph, std::vector<uint64_t> &colors,
                  const uint64_t vertex, const uint64_t new_color) {
  const uint64_t color_count = 2;
  if (colors[vertex] == 0) {
    colors[vertex] = new_color;
  }
  for (auto &current_edge : graph[vertex]) {
    if (colors[current_edge.vertex] != 0) {
      if (colors[vertex] == colors[current_edge.vertex])
        return false;
      continue;
    }
    if (!is_bipartite(graph, colors, current_edge.vertex,
                      (colors[vertex] % color_count) + 1))
      return false;
  }
  return true;
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
  std::vector<uint64_t> colors(vertex_count, 0);
  uint64_t color_count = 0;
  bool can_be_colored = is_bipartite(graph, colors, 0, 1);
  for (const auto &color : colors) {
    if (color != 0)
      ++color_count;
  }
  std::println(
      "Граф двудольный:{}",
      (color_count == vertex_count && can_be_colored ? "true" : "false"));
  return 0;
}
