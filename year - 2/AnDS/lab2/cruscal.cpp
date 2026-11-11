#include <iostream>
#include <print>
#include <vector>
#include <cstdint>
#include <climits>
#include <algorithm>
#include <numeric>

using namespace std;

// Вариант 1
/*
7 10 
0 1 7
0 3 5
1 2 8
1 4 7
2 4 5
3 4 15
3 5 6
4 5 8
4 6 9
5 6 11
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

class disjoint_set{
   vector <uint64_t> parents;
   vector <uint64_t> weight;
public:
   disjoint_set(uint64_t size) : parents(size), weight(size, 1){
      iota(parents.begin(), parents.end(), 0);
   }
   uint64_t find(const uint64_t index){
      if(parents[index] == index)return index;
      return parents[index] = this->find(parents[index]);
   }
   void merge(uint64_t first, uint64_t second){
      first  = this->find(first);
      second = this->find(second);
      if(weight[first] >= weight[second]){
	 parents[second] = first;
	 weight[first] += weight[second];
      }
      else{
	 parents[first] = second;
	 weight[second] += weight[first];
      }
   }
};

vector <edge> Cruscal(const vector <vector <halfedge>> &graph){
   disjoint_set trees(graph.size());
   vector <edge> mst;
   vector <edge> edges;
   for(uint64_t i = 0; i < graph.size(); i++){
      for(const auto& e : graph[i])edges.emplace_back(i, e);
   }
   sort(edges.begin(), edges.end());
   for(const auto& cur_edge : edges){
      if(trees.find(cur_edge.from) == trees.find(cur_edge.to))continue;
      trees.merge(cur_edge.from, cur_edge.to);
      mst.push_back(cur_edge);
      if(mst.size() == graph.size() - 1)break;
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
   }
   uint64_t sum = 0;
   println("Рёбра входящие в минимальное остовное дерево:"); 
   for(const edge& e: Cruscal(graph)){
      println("{} {} {}", e.from, e.to, e.weight);
      sum += e.weight;
   }
   println("Вес минимального остовного дерева:{}", sum);
   return 0;
}
