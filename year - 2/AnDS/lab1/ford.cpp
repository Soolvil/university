#include <iostream>
#include <queue>
#include <vector>
#include <cstdint>
#include <climits>

using namespace std;

// Вариант 11
/*
10 16
0 1 7
0 3 6
0 5 6
0 6 5
1 2 4
1 3 -2
2 7 -2
2 8 -4
3 4 2
3 6 1
5 6 -2
5 8 3
6 4 3
7 9 -1
8 7 1
9 8 1
0
*/


vector <uint64_t> bellmanFord(const uint64_t start, const vector <vector <pair<int64_t, int64_t>>>& graph){
   vector <bool> visited(graph.size());
   vector <int64_t> ranges(graph.size());
   vector <uint64_t> prev(graph.size());
   prev[0] = 0;
   ranges[start] = 0;
   visited[start] = true;
   bool changed = false;
   for(uint64_t k = 1; k < graph.size(); k++){
      changed = false;
      for(uint64_t i = 0; i < graph.size(); i++){
	 if(!visited[i])continue;
	 for(auto& cur_edge : graph[i]){
	    if(!visited[cur_edge.first]){
	       visited[cur_edge.first] = true;
	       ranges[cur_edge.first] = ranges[i] + cur_edge.second;
	       changed = true;
	       prev[cur_edge.first] = i;
	    }
	    else {
	       const int64_t new_range = ranges[i] + cur_edge.second;
	       if(ranges[cur_edge.first] > new_range){
		  ranges[cur_edge.first] = new_range;
		  changed = true;
		  prev[cur_edge.first] = i;
	       }
	    }
	 }
      }
      for(uint64_t i = 0; i < graph.size(); i++){
	 if(visited[i]){
	    cout << ranges[i] << " ";
	 }
	 else {
	    cout << "INF";
	 }
      }
      cout << "\n";
      if(!changed)break;
   }
   return prev;
}


int main(){
   ios_base::sync_with_stdio(false);
   uint64_t n, m;
   cin >> n >> m; 
   vector <vector <pair <int64_t, int64_t>>> graph(n);
   for(uint64_t i = 0; i < m; i++){
      int64_t from, to, weight;
      cin >> from >> to >> weight;
      graph[from].push_back({to, weight});
   }
   uint64_t start, goal;
   cin >> start;
   for(auto a : bellmanFord(start, graph)){
      cout << a << " ";
   }
   cout << "\n";
   return 0;
}
