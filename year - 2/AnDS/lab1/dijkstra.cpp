#include <iostream>
#include <queue>
#include <vector>
#include <cstdint>
#include <climits>

using namespace std;

// Вариант 11
/* 
8 12
0 1 2
0 3 5
0 4 4
1 2 4
1 5 2
1 6 2
2 3 4
2 6 6
3 7 3
4 5 2
5 6 4
6 7 8
0 7
*/
uint64_t Dijkstra(const uint64_t start, const uint64_t goal, const vector <vector <pair <uint64_t, uint64_t>>> &graph){
   vector <bool> visited(graph.size(), false);
   using pairT = pair<uint64_t, uint64_t>;
   priority_queue <pairT, vector<pairT>, std::greater<pairT>> squeue;
   vector <uint64_t> ranges(graph.size());
   vector <uint64_t> prev(graph.size());
   prev[0] = 0;
   squeue.push({0, start});
   while(!squeue.empty()){
      pairT cur = squeue.top();
      squeue.pop();
      if(visited[cur.second]){
	 continue;
      }
      visited[cur.second] = true;
      ranges[cur.second] = cur.first;
      for(auto edge : graph[cur.second]){
	 if(!visited[edge.first])prev[edge.first] = cur.second;
	 squeue.push({cur.first + edge.second, edge.first});
      }
   }
   for(auto r : ranges)cout << r << " ";
   cout << "\n";
   for(auto p : prev)cout << p << " ";
   cout << "\n";
   return ranges[goal];
}

int main(){
   ios_base::sync_with_stdio(false);
   uint64_t n, m;
   cin >> n >> m; 
   vector <vector <pair <uint64_t, uint64_t>>> graph(n);
   for(uint64_t i = 0; i < m; i++){
      uint64_t from, to, weight;
      cin >> from >> to >> weight;
      graph[from].push_back({to, weight});
   }
   uint64_t start, goal;
   cin >> start >> goal;
   cout << Dijkstra(start, goal, graph) << "\n";
   return 0;
}
