#include <concepts>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <print>
#include <queue>
#include <vector>

template <typename intT>
  requires(std::unsigned_integral<intT>)
struct code {
  intT val;
  uint8_t len;
};

template <typename intT>
  requires(std::unsigned_integral<intT>)
struct node {
  using value_type = code<intT>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  std::unique_ptr<node<intT>> lchild;
  std::unique_ptr<node<intT>> rchild;
  value_type val;
  size_t cnt;
  bool is_leaf() const noexcept {
    return lchild == nullptr && rchild == nullptr;
  }
  bool operator<(const node<intT>& rhs) const noexcept {
    return this->cnt < rhs.cnt;
  }
  bool operator>(const node<intT>& rhs) const noexcept {
    return this->cnt > rhs.cnt;
  }
  bool operator<=(const node<intT>& rhs) const noexcept {
    return this->cnt <= rhs.cnt;
  }
  bool operator>=(const node<intT>& rhs) const noexcept {
    return this->cnt >= rhs.cnt;
  }
};

template <typename T>
class cmp {
 public:
  bool operator()(T lhs, T rhs) { return (*lhs) > (*rhs); }
};

std::map<uint8_t, size_t> get_dict(std::string& str) {
  std::map<uint8_t, size_t> dict;
  for (auto& symbol : str) {
    dict[symbol]++;
  }
  return dict;
}

void set_len(std::unique_ptr<node<uint8_t>>& cur, uint8_t len) {
  cur->val.len = len;
  if (cur->is_leaf()) {
    return;
  }
  set_len(cur->lchild, len + 1);
  set_len(cur->rchild, len + 1);
}

std::unique_ptr<node<uint8_t>> gen_tree(std::map<uint8_t, size_t>& dict) {
  std::priority_queue<node<uint8_t>*, std::vector<node<uint8_t>*>,
                      cmp<node<uint8_t>*>>
      pqueue;
  for (auto& [val, cnt] : dict) {
    node<uint8_t>* nptr;
    nptr = new node<uint8_t>;
    nptr->val.val = val;
    nptr->val.len = 0;
    nptr->cnt = cnt;
    pqueue.push(nptr);
  }
  while (pqueue.size() > 1) {
    node<uint8_t>* rhs = pqueue.top();
    pqueue.pop();
    node<uint8_t>* lhs = pqueue.top();
    pqueue.pop();
    node<uint8_t>* nptr;
    nptr = new node<uint8_t>;
    nptr->val.val = 0;
    nptr->val.len = 0;
    nptr->cnt = lhs->cnt + rhs->cnt;
    nptr->lchild.reset(lhs);
    nptr->rchild.reset(rhs);
    pqueue.push(nptr);
  }
  std::unique_ptr<node<uint8_t>> res(pqueue.top());
  set_len(res, 0);
  return res;
}

std::pair<uint8_t, uint8_t> get_code(std::unique_ptr<node<uint8_t>>& cur,
                                     uint8_t val, uint8_t code) {
  if (cur->is_leaf()) {
    if (cur->val.val == val) {
      return {code, cur->val.len};
    }
    return {0, 0};
  }
  std::pair<uint8_t, uint8_t> temp;
  temp = get_code(cur->lchild, val, code);
  if (temp != std::pair<uint8_t, uint8_t>{0, 0}) {
    return temp;
  }
  return get_code(cur->rchild, val, (code | (1 << cur->val.len)));
}

std::vector<bool> encode(std::unique_ptr<node<uint8_t>>& tree,
                         std::string& str) {
  std::pair<uint8_t, uint8_t> coded;
  std::vector<bool> res;
  for (auto& symbol : str) {
    coded = get_code(tree, symbol, 0);
    for (size_t i = 0; i < coded.second; i++) {
      res.push_back(static_cast<bool>(coded.first & (1 << i)));
    }
  }
  return res;
}

void print_tree(std::unique_ptr<node<uint8_t>>& cur) {
  if (cur->is_leaf()) {
    std::println("{:x} - '{}' {}", reinterpret_cast<uintptr_t>(cur.get()),
                 static_cast<char>(cur->val.val), cur->cnt);
    return;
  }
  std::println("{:x} - {:x} {:x} {}", reinterpret_cast<uintptr_t>(cur.get()),
               reinterpret_cast<uintptr_t>(cur->lchild.get()),
               reinterpret_cast<uintptr_t>(cur->rchild.get()), cur->cnt);
  print_tree(cur->lchild);
  print_tree(cur->rchild);
}

void task1() {
  std::map<uint8_t, size_t> dict;
  dict['a'] = 31;
  dict['b'] = 26;
  dict['c'] = 24;
  dict['d'] = 18;
  dict['e'] = 1;
  std::unique_ptr<node<uint8_t>> tree = gen_tree(dict);
  for (auto& [symb, cnt] : dict) {
    std::string strcode;
    std::pair<uint8_t, uint8_t> symcode = get_code(tree, symb, 0);
    for (uint8_t i = 0; i < symcode.second; i++) {
      strcode += static_cast<char>((symcode.first & (1 << i)) ? 1 : 0) + '0';
    }
    std::println("{:c} - {}", symb, strcode);
  }
  std::println();
}

void task2() {
  std::string msg;
  std::getline(std::cin, msg);
  auto dict = get_dict(msg);
  auto tree = gen_tree(dict);
  auto coded = encode(tree, msg);
  for (size_t i = 0; i < coded.size(); i++) {
    std::cout << (coded[i] ? "1" : "0");
  }
  std::println();
  print_tree(tree);
  std::println();
}

void task3() {
  std::string msg = "Algorithms and data structures";
  auto dict = get_dict(msg);
  auto tree = gen_tree(dict);
  auto coded = encode(tree, msg);
  for (size_t i = 0; i < coded.size(); i++) {
    std::cout << (coded[i] ? "1" : "0");
  }
  std::println();
}

int main() {
  task1();
  task2();
  task3();
  return 0;
}
