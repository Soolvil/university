#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>
#include <utility>
#include <vector>

uint64_t swap_count = 0;
template <typename T>
void myswap(T& first, T& second) {
  swap_count++;
  std::swap(first, second);
}

uint64_t cmp_count = 0;
template <typename T>
bool less(const T& first, const T& second) {
  cmp_count++;
  return first < second;
}

template <typename T>
bool greater(const T& first, const T& second) {
  cmp_count++;
  return first > second;
}

template <typename T>
bool is_sorted(std::vector<T>& vect, const size_t left, const size_t right) {
  for (size_t i = left; i < right - 1; i++) {
    if (vect[i] > vect[i + 1]) return false;
  }
  return true;
}

template <typename T>
void shaker_sort(std::vector<T>& vect, size_t left, size_t right) {
  while (left + 1 < right) {
    size_t new_right = left;
    for (size_t i = left; i < right - 1; i++) {
      if (greater(vect[i], vect[i + 1])) {
        myswap(vect[i], vect[i + 1]);
        new_right = i + 1;
      }
    }
    right = new_right;
    size_t new_left = right;
    for (size_t i = right - 1; i > left; i--) {
      if (greater(vect[i - 1], vect[i])) {
        myswap(vect[i - 1], vect[i]);
        new_left = i;
      }
    }
    left = new_left;
  }
}

template <typename T>
void selection_sort(std::vector<T>& vect, size_t left, size_t right) {
  for (size_t i = left; i < right - 1; i++) {
    size_t min_iter = i;
    for (size_t j = i + 1; j < right; j++) {
      if (less(vect[j], vect[min_iter])) min_iter = j;
    }
    if (min_iter != i) myswap(vect[i], vect[min_iter]);
  }
}

template <typename T>
size_t binsearch(std::vector<T>& vect, size_t left, size_t right,
                 const T& key) {
  size_t mid;
  while (left + 1 < right) {
    mid = (right - left) / 2 + left - 1;
    if (greater(vect[mid], key)) {
      right = mid + 1;
    } else {
      left = mid + 1;
    }
  }
  return left;
}

template <typename T>
void insert_sort(std::vector<T>& vect, size_t left, size_t right) {
  for (size_t i = left + 1; i < right; i++) {
    size_t j = i;
    if (!greater(vect[j - 1], vect[j])) continue;
    size_t iter = binsearch(vect, left, i + 1, vect[j]);
    for (size_t k = j; k > iter; k--) {
      myswap(vect[k - 1], vect[k]);
    }
  }
}

template <typename T>
void comb_sort(std::vector<T>& vect, size_t left, size_t right) {
  size_t k = floor(static_cast<double>(right - left) / 1.3);
  while (k > 1) {
    for (size_t i = left; i + k < right; i++) {
      if (greater(vect[i], vect[i + k])) {
        myswap(vect[i], vect[i + k]);
      }
    }
    // std::cout << k << "\n";
    k = floor(static_cast<double>(k) / 1.3);
  }
  while (left < right) {
    size_t new_right = left;
    for (size_t i = left; i + 1 < right; i++) {
      if (greater(vect[i], vect[i + 1])) {
        myswap(vect[i], vect[i + 1]);
        new_right = i;
      }
    }
    right = new_right;
  }
}

template <typename T>
void shell_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  for (size_t h = (right - left) / 2; h > 0; h /= 2) {
    for (size_t i = left; i < left + h; i++) {
      for (size_t j = i + h; j < right; j += h) {
        size_t k = j;
        while (k >= left + h && greater(vect[k - h], vect[k])) {
          myswap(vect[k - h], vect[k]);
          k -= h;
        }
      }
    }
  }
}

template <typename T>
void sift_down(std::vector<T>& vect, size_t i, const size_t left,
               const size_t right) {
  if (i < left) return;
  while (i * 2 + 1 < right) {
    size_t lchild = i * 2 + 1;
    size_t rchild = i * 2 + 2;
    size_t j = lchild;
    if (rchild < right && greater(vect[rchild], vect[lchild])) {
      j = rchild;
    }
    if (!less(vect[i], vect[j])) return;
    myswap(vect[i], vect[j]);
    i = j;
  }
}

template <typename T>
void heapify(std::vector<T>& vect, const size_t left, const size_t right) {
  for (size_t i = (right - left) / 2 + left; i > 0; i--) {
    sift_down(vect, i, left, right);
  }
  sift_down(vect, 0, left, right);
}

template <typename T>
void heap_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  heapify(vect, left, right);
  size_t sorted_count = 0;
  while (right > left + sorted_count) {
    myswap(vect[left], vect[right - sorted_count - 1]);
    sorted_count++;
    sift_down(vect, left, left, right - sorted_count);
  }
}

template <typename T>
void merge_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  if (left >= right - 1) return;
  size_t mid = (right - left) / 2 + left;
  merge_sort(vect, left, mid);
  merge_sort(vect, mid, right);
  std::vector<T> buff;
  size_t i = left;
  size_t j = mid;
  while (i < mid && j < right) {
    if (less(vect[i], vect[j])) {
      buff.push_back(vect[i]);
      i++;
    } else {
      buff.push_back(vect[j]);
      j++;
    }
  }
  while (i < mid) {
    buff.push_back(vect[i]);
    i++;
  }
  while (j < right) {
    buff.push_back(vect[j]);
    j++;
  }
  for (size_t k = 0; k < right - left; k++) {
    vect[k + left] = buff[k];
  }
  return;
}

template <typename T>
size_t partition(std::vector<T>& vect, const size_t left, const size_t right) {
  size_t i = left;
  size_t j = right;
  size_t mid = (right + left) / 2;
  T& pivot = vect[mid];
  while (i <= j) {
    while (less(vect[i], pivot)) i++;
    while (greater(vect[j], pivot)) j--;
    if (j <= i) return j;
    myswap(vect[i], vect[j]);
    i++;
    j--;
  }
  return j;
}

template <typename T>
void quick_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  if (left + 1 >= right) return;
  if (right - left == 2) {
    if (greater(vect[left], vect[left + 1])) myswap(vect[left], vect[left + 1]);
    return;
  }
  size_t sep = partition(vect, left, right - 1);
  quick_sort(vect, left, sep + 1);
  quick_sort(vect, sep + 1, right);
}

template <typename T>
void quick_shell_sort(std::vector<T>& vect, const size_t left,
                      const size_t right) {
  if (left + 1 >= right) return;
  if (left + 10 >= right) {
    shell_sort(vect, left, right);
    return;
  }
  size_t sep = partition(vect, left, right);
  quick_shell_sort(vect, left, sep);
  quick_shell_sort(vect, sep, right);
}

std::atomic<size_t> thread_count;
template <typename T>
void threaded_quick_sort(std::vector<T>& vect, const size_t left,
                         const size_t right) noexcept {
  if (left + 1 >= right) return;
  if (right - left <= 16) {
    shell_sort(vect, left, right);
    return;
  }
  if (right - left <= 100000) {
    quick_shell_sort(vect, left, right);
    return;
  }
  size_t sep = partition(vect, left, right);
  if (thread_count >= 8) {
    threaded_quick_sort(vect, left, sep);
    threaded_quick_sort(vect, sep, right);
    return;
  }
  if (sep - left >= right - sep) {
    thread_count++;
    std::thread tr(threaded_quick_sort<T>, std::ref(vect), sep, right);
    threaded_quick_sort(vect, left, sep);
    tr.join();
    thread_count--;
  } else {
    thread_count++;
    std::thread tr(threaded_quick_sort<T>, std::ref(vect), left, sep);
    threaded_quick_sort(vect, sep, right);
    tr.join();
    thread_count--;
  }
}

template <typename T>
void tmerge_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  if (left >= right - 1) return;
  if (right - left <= 16) {
    comb_sort(vect, left, right);
    return;
  }
  if (right - left <= 100000) {
    merge_sort(vect, left, right);
    return;
  }
  size_t mid = (right - left) / 2 + left;
  if (thread_count < 8) {
    thread_count++;
    std::thread tr(tmerge_sort<T>, std::ref(vect), left, mid);
    tmerge_sort(vect, mid, right);
    tr.join();
    thread_count--;
  } else {
    tmerge_sort(vect, left, mid);
    tmerge_sort(vect, mid, right);
  }
  std::vector<T> buff;
  buff.reserve(right - left);
  size_t i = left;
  size_t j = mid;
  while (i < mid && j < right) {
    if (less(vect[i], vect[j])) {
      buff.push_back(vect[i]);
      i++;
    } else {
      buff.push_back(vect[j]);
      j++;
    }
  }
  while (i < mid) {
    buff.push_back(vect[i]);
    i++;
  }
  while (j < mid) {
    buff.push_back(vect[j]);
    j++;
  }
  for (size_t k = 0; k < right - left; k++) {
    vect[k + left] = buff[k];
  }
  return;
}

#ifdef ESOTERICSORT
template <typename T>
void slow_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  if (left + 1 >= right) return;
  const size_t mid = (right - left) / 2 + left;
  slow_sort(vect, left, mid);
  slow_sort(vect, mid, right);
  if (greater(vect[mid - 1], vect[right - 1]))
    myswap(vect[mid - 1], vect[right - 1]);
  slow_sort(vect, left, right - 1);
}
#endif

#ifdef ESOTERICSORT
template <typename T>
void bogo_sort(std::vector<T>& vect, const size_t left, const size_t right) {
  std::mt19937_64 gen(time(0));
  auto ileft = vect.begin();
  std::advance(ileft, left);
  auto iright = vect.begin();
  std::advance(iright, right);
  while (!is_sorted(vect, left, right)) {
    std::shuffle(ileft, iright, gen);
  }
}
#endif

template <typename T>
void print_sort(std::vector<T> vect,
                void (*sorter)(std::vector<T>&, size_t, size_t)) {
  swap_count = 0;
  cmp_count = 0;
  auto start = std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
  sorter(vect, 0, vect.size());
  auto end = std::chrono::duration_cast<std::chrono::microseconds>(
                 std::chrono::high_resolution_clock::now().time_since_epoch())
                 .count();
  // for(size_t i = 0; i < vect.size(); i++)std::cout << vect[i] << " ";
  // std::cout << "\n";
  std::cout << "swaps        = " << swap_count << "\n";
  std::cout << "comparations = " << cmp_count << "\n";
  std::cout << "time         = " << static_cast<double>(end - start) / 1000000
            << " s\n";
  std::cout << "sorted       = "
            << (is_sorted(vect, 0, vect.size()) ? "true\n" : "false\n");
  std::cout << "\n";
  std::cout.flush();
}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin.tie(0);
  thread_count = 0;
  std::mt19937_64 gen(time(0));
  size_t vect_size = 0;
  std::cin >> vect_size;
  std::vector<int64_t> vect(vect_size);
  for (size_t i = 0; i < vect_size; i++) vect[i] = gen() % 100;
  // for(size_t i = 0; i < vect_size; i++)std::cout << vect[i] << " ";
  // std::cout << "\n";
  std::cout << "\n";
#ifndef NSQUARESORT
  std::cout << "Shaker sort:\n";
  print_sort(vect, shaker_sort);
  std::cout << "Selection sort:\n";
  print_sort(vect, selection_sort);
  std::cout << "Insert sort:\n";
  print_sort(vect, insert_sort);
#endif
  std::cout << "Comb sort:\n";
  print_sort(vect, comb_sort);
  std::cout << "Shell sort:\n";
  print_sort(vect, shell_sort);
  std::cout << "Heap sort:\n";
  print_sort(vect, heap_sort);
  std::cout << "Merge sort:\n";
  print_sort(vect, merge_sort);
  std::cout << "Quick sort:\n";
  print_sort(vect, quick_sort);
#ifndef NSQUARESORT
  std::cout << "Quick Shell sort:\n";
  print_sort(vect, quick_shell_sort);
  std::cout << "Thread Quick sort:\n";
  print_sort(vect, threaded_quick_sort);
  std::cout << "Thread Merge sort:\n";
  print_sort(vect, tmerge_sort);
#endif
#ifdef ESOTERICSORT
  std::cout << "Slow sort:\n";
  print_sort(vect, slow_sort);
  std::cout << "Bogo sort:\n";
  print_sort(vect, bogo_sort);
#endif
  return 0;
}
