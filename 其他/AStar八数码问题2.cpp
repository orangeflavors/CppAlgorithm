#include <bits/stdc++.h>
using namespace std;
string value(const vector<vector<int>> &board) {
  string res = "";
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) res += board[i][j] + '0';
  return res;
}
bool checker(const vector<vector<int>> &board,
             const vector<vector<int>> &target) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (board[i][j] != target[i][j]) return false;
  return true;
}
int evaluate(int step, const vector<vector<int>> &board,
             const vector<vector<int>> &target) {
  int res = step;
  // for (int i = 0; i < 3; i++)
  //   for (int j = 0; j < 3; j++)
  //     if (board[i][j] != target[i][j]) {   // 不在正确位置上的元素个数
  //       res++;
  //     }
  int old_x, old_y, end_x, end_y;
  for (int k = 1; k < 9; k++) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board[i][j] == k) {
          old_x = i;
          old_y = j;
        }
        if (target[i][j] == k) {              // 曼哈顿距离
          end_x = i;
          end_y = j;
        }
      }
    }
    res += abs(old_x - end_x) + abs(old_y - end_y);
  }
  return res;
}
void printer(const vector<vector<int>> &board) {
  cout << "---------" << endl;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) cout << board[i][j] << " ";
    cout << endl;
  }
  cout << "---------" << endl;
}
int main() {
//  vector<vector<int>> board = {{5, 0, 8}, {4, 2, 1}, {7, 3, 6}};
//  vector<vector<int>> target = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
//  vector<pair<int, int>> move = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};  
  vector<vector<int>> board = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
  vector<vector<int>> target = {{1, 2, 3}, {0, 4, 6}, {7, 5, 8}};
  vector<pair<int, int>> move = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
  auto isOutside = [&](int x, int y) {
    return x < 0 || x > 2 || y < 0 || y > 2;
  };
  priority_queue<tuple<int, int, vector<vector<int>>, vector<string>>,
                 vector<tuple<int, int, vector<vector<int>>, vector<string>>>,
                 greater<>>
      pq; // 优先队列，greater 用于升序排列，保证 top 是最小的元素
  pq.emplace(evaluate(0, board, target), 0, board, vector<string>{});
  map<string, bool> vis;
  int step = 0;
  printer(board);
  tuple<int, int, vector<vector<int>>, vector<string>> res;
  while (!pq.empty()) {
    //  估值 路径长度 当前状态 历史状态
    auto [v, st, now, his] = pq.top();
    pq.pop();
    string hashv = value(now);
    if (vis.count(hashv)) continue;
    vis[hashv] = true;
    cout << "v: " << v << " step: " << st << endl;
    printer(now);
    if (checker(now, target)) {
      cout << "v: " << v << " total step: " << step << " path step: " << st
           << endl;
      printer(now);
      res = {v, st, now, his};
      break;
    }
    step++;
    int x, y;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        if (now[i][j] == 0) x = i, y = j;
    for (int i = 0; i < 4; i++) {
      auto [dx, dy] = move[i];
      if (isOutside(x + dx, y + dy)) continue;
      vector<vector<int>> co(now);
      swap(co[x][y], co[x + dx][y + dy]);
      if (vis.count(value(co))) continue;
      vector<string> newHis(his);
      newHis.push_back(hashv);
      int ev = evaluate(st + 1, co, target);
      pq.emplace(ev, st + 1, co, newHis);
    }
  }
  auto [v, st, now, his] = res;
  for (auto t = his.size() - his.size(); t < his.size(); t++) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) cout << his[t][i * 3 + j] << " ";
      if (i == 1) cout << " " << t;
      cout << endl;
    }
    cout << "---------" << endl;
  }
  return 0;
}
