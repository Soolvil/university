#include <iostream>
#include <cstdint>
#include <vector>

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
size_t partition(std::vector <T>& vect, const size_t left, const size_t right){
   size_t i = left;
   size_t j = right;
   T& pivot = vect[(right - left) / 2 + left];
   while(j > i){
      while(less(vect[i], pivot))i++;
      while(greater(vect[j], pivot))j--;
      if(j <= i)return j;
      myswap(vect[i++], vect[j--]);
   }
   return j;
}

template <typename T>
void quick_sort(std::vector <T>& vect, const size_t left, const size_t right){
   if(left >= right)return;
   size_t sep = partition(vect, left, right);
   quick_sort(vect, left, sep);
   quick_sort(vect, sep + 1, right);
}

int main(){
   std::ios_base::sync_with_stdio(0);
   std::cin.tie(0);
   size_t vect_size = 0;
   std::cin >> vect_size;
   std::vector <int64_t> vect(vect_size);
   for(size_t i = 0; i < vect_size; i++)std::cin >> vect[i];
   quick_sort(vect, 0, vect_size - 1);
   for(size_t i = 0; i < vect_size; i++)std::cout << vect[i] << " ";
   std::cout << "\n";
   std::cout << "swaps        = " << swap_count << "\n";
   std::cout << "comparations = " << cmp_count  << "\n";
   return 0;
}
