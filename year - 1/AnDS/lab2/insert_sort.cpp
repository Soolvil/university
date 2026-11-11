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
void insert_sort(std::vector <T>& vect, size_t left, size_t right){
   for(size_t i = left + 1; i < right; i++){
      size_t j = i;
      while(j > left && greater(vect[j - 1], vect[j])){
	 myswap(vect[j - 1], vect[j]);
	 j--;
      }
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
