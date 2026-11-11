#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

// Задание 10.1

struct coords {
  uint64_t x;
  uint64_t y;
};

vector<pair<int64_t, int64_t>> moves = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0},
};

bool DFS(coords there, coords goal, uint64_t num,
         vector<vector<uint64_t>>& matrix, vector<vector<bool>>& visited) {
  visited[there.y][there.x] = true;
  // cout << there.y << " " << there.x << "\n";
  if (there.x == goal.x && there.y == goal.y) {
    matrix[there.y][there.x] = num + 1;
    return true;
  }
  for (size_t i = 0; i < 4; i++) {
    coords next;
    next.x = there.x + moves[i].second;
    next.y = there.y + moves[i].first;
    if (visited[next.y][next.x]) continue;
    if (matrix[next.y][next.x] != 0) continue;
    if (DFS(next, goal, num, matrix, visited)) {
      matrix[there.y][there.x] = num + 1;
      return true;
    }
  }
  visited[there.y][there.x] = false;
  return false;
}

int main() {
  size_t n, m, k;
  cin >> n >> m >> k;
  vector<vector<uint64_t>> matrix(n + 2, vector<uint64_t>(m + 2, 1));
  vector<uint64_t> rows(n, 0);
  for (size_t i = 2; i < n; i++) {
    for (size_t j = 1; j <= m; j++) {
      char temp;
      cin >> temp;
      if (temp == '0') {
        matrix[i][j] = 0;
        rows[i - 1]++;
      }
    }
  }
  vector<pair<uint64_t, uint64_t>> person(k);
  for (size_t i = 0; i < k; i++) {
    cin >> person[i].first >> person[i].second;
    matrix[1][person[i].first] = 0;
    matrix[n][person[i].second] = 0;
  }
  sort(person.begin(), person.end());
  vector<vector<bool>> visited(n + 2, vector<bool>(m + 2, false));
  for (size_t i = 1; i < n - 1; i++) {
    if (rows[i] < k) goto fail;
  }
  for (size_t i = 1; i < k; i++) {
    if (person[i - 1].second > person[i].second) goto fail;
  }
  for (size_t i = 0; i < k; i++) {
    coords start;
    coords goal;
    start.y = 1;
    start.x = person[i].first;
    goal.y = n;
    goal.x = person[i].second;
    if (!DFS(start, goal, i + 1, matrix, visited)) {
      goto fail;
    }
  }
  cout << "Possible\n";
  for (size_t i = 1; i <= n; i++) {
    for (size_t j = 1; j <= m; j++) {
      switch (matrix[i][j]) {
        case 0:
          cout << ".";
          break;
        case 1:
          cout << "#";
          break;
        default:
          cout << matrix[i][j] - 1;
      }
    }
    cout << "\n";
  }
  return 0;
fail:
  cout << "Impossible\n";
  return 0;
}
