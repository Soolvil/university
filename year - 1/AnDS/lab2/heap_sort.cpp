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
void sift_down(std::vector <T>& vect, size_t i, const size_t left, const size_t right){
   if(i < left)return;
   while(i * 2 + 1 < right){
      size_t lchild = i * 2 + 1;
      size_t rchild = i * 2 + 2;
      size_t j = lchild;
      if(rchild < right && greater(vect[rchild], vect[lchild])){
	 j = rchild;
      }
      if(!less(vect[i], vect[j]))return;
      myswap(vect[i], vect[j]);
      i = j;
   }
}

template <typename T>
void heapify(std::vector <T>& vect, const size_t left, const size_t right){
   for(size_t i = (right - left) / 2 + left; i > 0; i--){
      sift_down(vect, i, left, right);
   }
   sift_down(vect, 0, left, right);
}

template <typename T>
void heap_sort(std::vector <T>& vect, const size_t left, const size_t right){
   heapify(vect, left, right);
   size_t sorted_count = 0;
   while(right > left + sorted_count){
      myswap(vect[left], vect[right - sorted_count - 1]);
      sorted_count++;
      sift_down(vect, left, left, right - sorted_count);
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
   heap_sort(vect, 0, vect_size);
   for(size_t i = 0; i < vect_size; i++)std::cout << vect[i] << " ";
   std::cout << "\n";
   std::cout << "swaps        = " << swap_count << "\n";
   std::cout << "comparations = " << cmp_count  << "\n";
   return 0;
}
