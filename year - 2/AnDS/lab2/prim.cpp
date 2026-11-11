#include <iostream>
#include <print>
#include <queue>
#include <vector>
#include <cstdint>
#include <climits>

using namespace std;

// Вариант 1
/*
8 13
0 1 4
0 3 10
0 4 2
1 2 18
1 3 8
2 3 11
2 7 19
3 4 5
3 6 11
3 7 9
4 5 51
5 6 1
5 7 2
*/
struct halfedge{
   uint64_t vertex;
   int64_t weight;
   halfedge(uint64_t _vertex = 0, uint64_t _weight = 0) : vertex(_vertex), weight(_weight) {};
   bool operator<(const halfedge& other) const{
      return this->weight < other.weight;
   }
   bool operator>(const halfedge& other) const{
      return this->weight > other.weight;
   }
   bool operator<=(const halfedge& other) const{
      return this->weight <= other.weight;
   }
   bool operator>=(const halfedge& other) const{
      return this->weight >= other.weight;
   }
};

struct edge{
   uint64_t from;
   uint64_t to;
   int64_t weight;
   edge(uint64_t _from = 0, uint64_t _to = 0, uint64_t _weight = 0) : from(_from), to(_to), weight(_weight) {};
   edge(halfedge _half, uint64_t _to) : from(_half.vertex), to(_to), weight(_half.weight) {};
   edge(uint64_t _from, halfedge _half) : from(_from), to(_half.vertex), weight(_half.weight) {};
   bool operator<(const edge& other) const{
      return this->weight < other.weight;
   }
   bool operator>(const edge& other) const{
      return this->weight > other.weight;
   }
   bool operator<=(const edge& other) const{
      return this->weight <= other.weight;
   }
   bool operator>=(const edge& other) const{
      return this->weight >= other.weight;
   }
};

vector <edge> Prim(const uint64_t start, const vector <vector <halfedge>> &graph){
   vector <bool> used(graph.size(), false);
   priority_queue <edge, vector<edge>, std::greater<edge>> squeue;
   vector <edge> mst;
   used[start] = true;
   for(const auto& cur_edge : graph[start])squeue.push(edge(start, cur_edge));
   while(!squeue.empty()){
      edge cur = squeue.top();
      squeue.pop();
      if(used[cur.to]){
	 continue;
      }
      used[cur.to] = true;
      mst.push_back(cur);
      if(mst.size() == graph.size() - 1)break;
      for(const auto& cur_edge : graph[cur.to]){
	 squeue.push(edge(cur.to ,cur_edge));
      }
   }
   return mst;
};

int main(){
   ios_base::sync_with_stdio(false);
   uint64_t n, m;
   cin >> n >> m; 
   vector <vector <halfedge>> graph(n);
   for(uint64_t i = 0; i < m; i++){
      uint64_t from, to, weight;
      cin >> from >> to >> weight;
      graph[from].push_back(halfedge(to, weight));
      graph[to].push_back(halfedge(from, weight));
   }
   uint64_t start = 0;
   uint64_t sum = 0;
   println("Рёбра входящие в минимальное остовное дерево:"); 
   for(const edge& e: Prim(start, graph)){
      println("{} {} {}", e.from + 1, e.to + 1, e.weight);
      sum += e.weight;
   }
   println("Вес минимального остовного дерева:{}", sum);
   return 0;
}
