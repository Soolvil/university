#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <ctime>

uint64_t swap_count = 0;
template <typename T>
void myswap(T& first, T& second){
   swap_count++;
   std::swap(first, second);
}

uint64_t cmp_count = 0;
template <typename T>
bool less(const T& first, const T& second){
   cmp_count++;
   return first < second;
}

template <typename T>
bool greater(const T& first, const T& second){
   cmp_count++;
   return first > second;
}

template <typename T>
size_t binsearch(std::vector <T>& vect, size_t left, size_t right, const T& key){
   size_t mid;
   while(left + 1 < right){
      //std::cout << "   " << left << " " << right << "\n";
      mid = (right - left) / 2 + left - 1;
      if(greater(vect[mid], key)){
	 right = mid + 1;
      }
      else {
      	 left = mid + 1;
      }
   }
   return left;
}

template <typename T>
void insert_sort(std::vector <T>& vect, size_t left, size_t right){
   for(size_t i = left + 1; i < right; i++){
      size_t j = i;
      if(!greater(vect[j - 1], vect[j]))continue;
      size_t iter = binsearch(vect, left, i + 1, vect[j]);
      //std::cout << j << " " << iter << "\n";
      for(size_t k = j; k > iter; k--){
	 myswap(vect[k - 1], vect[k]);
      }
      //for(auto& v : vect)std::cout << v << " ";
      //std::cout << "\n";
   }
}

int main(){
   std::ios_base::sync_with_stdio(0);
   std::cin.tie(0);
   std::mt19937_64 gen(time(0));
   size_t vect_size = 0;
   std::cin >> vect_size;
   std::vector <int64_t> vect(vect_size);
   for(size_t i = 0; i < vect_size; i++)vect[i] = gen();
   insert_sort(vect, 0, vect.size());
   for(size_t i = 0; i < vect_size; i++)std::cout << vect[i] << " ";
   std::cout << "\n";
   std::cout << "swaps        = " << swap_count << "\n";
   std::cout << "comparations = " << cmp_count  << "\n";
   return 0;
}
