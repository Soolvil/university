#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

size_t distance(const std::string& a, const std::string& b){
   if(a.size() == 0)return b.size();
   if(b.size() == 0)return a.size();
   std::vector <std::vector<uint64_t>> result(a.size() + 1, std::vector<uint64_t>(b.size() + 1, 0));
   result[0][0] = 0;
   for(size_t i = 1; i <= a.size(); i++)result[i][0] = result[i - 1][0] + 1;
   for(size_t i = 1; i <= b.size(); i++)result[0][i] = result[0][i - 1] + 1;
   for(size_t i = 1; i <= a.size(); i++){
      for(size_t j = 1; j <= b.size(); j++){
	 result[i][j] = std::min(result[i - 1][j], result[i][j - 1]) + 1;
	 result[i][j] = std::min(result[i][j], result[i - 1][j - 1] + (a[i - 1] == b[j - 1] ? 0 : 1));
      }
   }
   for(auto ra : result){
      for(auto rb : ra){
	 std::cout << rb << " ";
      }
      std::cout << "\n";
   }
   return result[a.size()][b.size()];
}

int main(){
   std::string str1, str2;
   std::cin >> str1 >> str2;
   std::cout << distance(str1, str2) << "\n";
}
