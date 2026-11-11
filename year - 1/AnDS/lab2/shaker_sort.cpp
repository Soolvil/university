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
void shaker_sort(std::vector <T>& vect, size_t left, size_t right){
   while(left < right){
      size_t new_right = left;
      for(size_t i = left; i < right - 1; i++){
	 if(greater(vect[i], vect[i + 1])){
	    myswap(vect[i], vect[i + 1]);
	    new_right = i + 1;
	 }
      }
      size_t new_left = right;
      for(size_t i = right - 1; i > left; i--){
	 if(greater(vect[i - 1], vect[i])){
	    myswap(vect[i - 1], vect[i]);
	    new_left = i;
	 }
      }
      right = new_right;
      left  = new_left;
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
   shaker_sort(vect, 0, vect.size());
   for(auto &v : vect)std::cout << v << " ";
   std::cout << "\n";
   std::cout << "swaps        = " << swap_count << "\n";
   std::cout << "comparations = " << cmp_count  << "\n";
   return 0;
}
