#include <iostream>
#include <queue>
#include <vector>
#include <cstdint>
#include <climits>

using namespace std;

// Вариант 11
/* 
4 9
0 1 4
0 2 5
0 3 6
1 0 5
1 3 1
2 0 3
2 3 4
3 1 2
3 2 2
*/

vector <vector <int64_t>> floyd(const vector <vector <pair <int64_t, int64_t>>>& graph){
   vector <vector <int64_t>> ranges(graph.size(), vector <int64_t>(graph.size(), 10000));
   for(uint64_t i = 0; i < graph.size(); i++)ranges[i][i] = 0;
   for(uint64_t i = 0; i < graph.size(); i++){
      for(uint64_t j = 0; j < graph[i].size(); j++){
	 ranges[i][graph[i][j].first] = graph[i][j].second;
      }
   }
   for(uint64_t k = 0; k < graph.size(); k++){
      for(uint64_t i = 0; i < graph.size(); i++){
	 for(uint64_t j = 0; j < graph.size(); j++){
	    ranges[i][j] = min(ranges[i][j], ranges[i][k] + ranges[k][j]);
	 }
      }
   }
   return ranges;
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
   for(auto& row : floyd(graph)){
      for(auto& r : row){
	 cout << r << " ";
      }
      cout << "\n";
   }
   return 0;
}
