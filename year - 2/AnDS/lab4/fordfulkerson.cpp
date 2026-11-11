#include <cstdint>
#include <iostream>
#include <limits>
#include <print>
#include <vector>

/* Вариант 1
6 8
0 1 3
0 3 5
1 2 5
2 4 6
2 5 5
3 1 4
3 4 2
4 5 7
*/

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

uint64_t dfs(graph_t &graph, std::vector<bool> &visited, const uint64_t sink,
             const uint64_t prev_vertex, const uint64_t curr_vertex,
             const uint64_t capacity) {
  auto update_edge = [&](const uint64_t add) {
    for (auto &current_edge : graph[prev_vertex]) {
      if (current_edge.vertex != curr_vertex)
        continue;
      current_edge.weight -= add;
    }
    for (auto &current_edge : graph[curr_vertex]) {
      if (current_edge.vertex != prev_vertex)
        continue;
      current_edge.weight += add;
    }
  };
  if (capacity == 0)
    return 0;
  if (curr_vertex == sink) {
    update_edge(capacity);
    return capacity;
  }
  visited[curr_vertex] = true;
  for (auto &search_edge : graph[curr_vertex]) {
    if (visited[search_edge.vertex])
      continue;
    uint64_t add;
    add = dfs(graph, visited, sink, curr_vertex, search_edge.vertex,
              std::min(capacity, static_cast<uint64_t>(search_edge.weight)));
    if (add == 0)
      continue;
    visited[curr_vertex] = false;
    if (prev_vertex != curr_vertex)
      update_edge(add);
    return add;
  }
  visited[curr_vertex] = false;
  return 0;
}

uint64_t ford_fulkerson(const graph_t &graph, const uint64_t source,
                        const uint64_t sink) {
  graph_t flow_network(graph.size());
  for (uint64_t i = 0; i < graph.size(); i++) {
    for (const auto &current_edge : graph[i]) {
      flow_network[i].push_back(current_edge);
      const halfedge reverse_edge = halfedge(i, 0);
      flow_network[current_edge.vertex].push_back(reverse_edge);
    }
  }
  std::vector<bool> visited(graph.size(), false);
  uint64_t res_flow = 0;
  while (true) {
    uint64_t add;
    add = dfs(flow_network, visited, sink, source, source,
              std::numeric_limits<uint64_t>::max());
    if (add == 0)
      break;
    res_flow += add;
  }
  return res_flow;
}

int main() {
  uint64_t vertex_count;
  uint64_t edge_count;
  std::cin >> vertex_count >> edge_count;
  graph_t graph(vertex_count);
  for (uint64_t i = 0; i < edge_count; i++) {
    uint64_t from;
    uint64_t to;
    uint64_t capacity;
    std::cin >> from >> to >> capacity;
    graph[from].push_back(halfedge(to, capacity));
  }
  std::println("Максимальный поток:{}",
               ford_fulkerson(graph, 0, vertex_count - 1));
  return 0;
}
