#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>

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

template <class TIter>
void shaker_sort(TIter first, TIter last){
   while(first != last){
      TIter new_last = first;
      for(TIter i = first; i != last; i++){
	 if(greater(*i, *next(i))){
	    myswap(*i, *next(i));
	    new_right = next(i);
	 }
      }
      last = new_last;
      TIter new_first = last;
      for(size_t i = last; i > left; i--){
	 if(greater(*prev(i), next())){
	    myswap(vect[i - 1], vect[i]);
	    new_left = i;
	 }
      }
      left  = new_left;
   }
}

template <typename T>
void selection_sort(std::vector <T>& vect, size_t left, size_t right){
   for(size_t i = left; i < right - 1; i++){
      size_t min_iter = i;
      for(size_t j = i + 1; j < right; j++){
	 if(less(vect[j], vect[min_iter]))min_iter = j;
      }
      if(min_iter != i)myswap(vect[i], vect[min_iter]);
   }
}

template <typename T>
size_t binsearch(std::vector <T>& vect, size_t left, size_t right, const T& key){
   size_t mid;
   while(left + 1 < right){
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
      for(size_t k = j; k > iter; k--){
	 myswap(vect[k - 1], vect[k]);
      }
   }
}

template <typename T>
void comb_sort(std::vector <T>& vect, size_t left, size_t right){
   size_t k = floor(static_cast<double>(right - left) / 1.3);
   while(k > 1){
      for(size_t i = left; i + k < right; i++){
	 if(greater(vect[i], vect[i + k])){
	    myswap(vect[i], vect[i + k]);
	 }
      }
      //std::cout << k << "\n";
      k = floor(static_cast<double>(k) / 1.3);
   }
   while(left < right){
      size_t new_right = left;
      for(size_t i = left; i + 1 < right; i++){
	 if(greater(vect[i], vect[i + 1])){
	    myswap(vect[i], vect[i + 1]);
	    new_right = i;
	 }
      }
      right = new_right;
   }
   return;
}

template <typename T>
void shell_sort(std::vector <T>& vect, const size_t left, const size_t right){
   for(size_t h = (right - left) / 2; h > 0; h /= 2){
      for(size_t i = left; i < left + h; i++){
	 for(size_t j = i + h; j < right; j += h){
	    size_t k = j;
	    while(k > left + h && greater(vect[k - h], vect[k])){
	       myswap(vect[k - h], vect[k]);
	       k -= h;
	    }
	 }
      }   
   }
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

template <typename T>
size_t partition(std::vector <T>& vect, const size_t left, const size_t right){
   size_t i = left;
   size_t j = right;
   size_t mid = (right - left) / 2 + left;
   T& pivot = vect[mid];
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
   if(left + 1 >= right)return;
   size_t sep = partition(vect, left, right - 1);
   quick_sort(vect, left, sep + 1);
   quick_sort(vect, sep + 1, right);
}

template <typename T>
void quick_shell_sort(std::vector <T>& vect, const size_t left, const size_t right){
   if(left + 1 >= right)return;
   if(left + 10 >= right){
      shell_sort(vect, left, right);
      return;
   }
   size_t sep = partition(vect, left, right - 1);
   quick_shell_sort(vect, left, sep + 1);
   quick_shell_sort(vect, sep + 1, right);
   return;
}

template <typename T>
void quick_comb_sort(std::vector <T>& vect, const size_t left, const size_t right){
   if(left + 1 >= right)return;
   if(left + 10 >= right){
      comb_sort(vect, left, right);
      return;
   }
   size_t sep = partition(vect, left, right - 1);
   quick_comb_sort(vect, left, sep + 1);
   quick_comb_sort(vect, sep + 1, right);
   return;
}

template <typename T>
void print_sort(std::vector <T> vect, void (*sorter)(std::vector <T>&, const size_t, const size_t)){
   swap_count = 0; cmp_count = 0;
   sorter(vect, 0, vect.size());
   //for(size_t i = 0; i < vect.size(); i++)std::cout << vect[i] << " ";
   //std::cout << "\n";
   std::cout << "swaps        = " << swap_count << "\n";
   std::cout << "comparations = " << cmp_count  << "\n";
}

int main(){
   std::ios_base::sync_with_stdio(0);
   std::cin.tie(0);
   std::mt19937_64 gen(time(0));
   size_t vect_size = 0;
   std::cin >> vect_size;
   std::vector <int64_t> vect(vect_size);
   for(size_t i = 0; i < vect_size; i++)vect[i] = gen() % 1000000;
   
   for(size_t i = 0; i < vect_size; i++)std::cout << vect[i] << " ";
   std::cout << "\n";
   std::cout << "Shaker sort:\n"; 
   //print_sort(vect, shaker_sort);
   std::cout << "Selection sort:\n";
   //print_sort(vect, selection_sort);
   std::cout << "Insert sort:\n";
   //print_sort(vect, insert_sort);
   std::cout << "Comb sort:\n"; 
   //print_sort(vect, comb_sort);
   std::cout << "Shell sort:\n";
   //print_sort(vect, shell_sort);
   std::cout << "Heap sort:\n";
   //print_sort(vect, heap_sort);
   std::cout << "Quick sort:\n";
   //print_sort(vect, quick_sort);
   std::cout << "Quick Shell sort:\n";
   //print_sort(vect, quick_shell_sort);
   std::cout << "Quick Comb sort:\n";
   //print_sort(vect, quick_comb_sort);
   return 0;
}
