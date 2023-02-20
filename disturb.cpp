#include <bits/stdc++.h>
using namespace std;
int ty[65][65];
int iss[65][65];
double a[65][65][4];

int main(int argc, char *argv[]) {
  int d = atoi(argv[1]);
  cout << d << '\n';
  int n;
  freopen("terrain.in", "r", stdin);
  cin >> n;
  vector<pair<int, int>> vec;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cin >> ty[i][j];
      vec.push_back(make_pair(i, j));
    }
  }
  srand(int(time(0)));
  random_shuffle(vec.begin(), vec.end());
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      for (int k = 0; k < 3; k++) {
        cin >> a[i][j][k];
      }
      cin >> iss[i][j];
      if (iss[i][j]) {
        cin >> a[i][j][3];
      }
    }
  }
  for (int i = 0; i < d; i++) {
    auto [x, y] = vec[i];
    a[x][y][3] = 500;
    iss[x][y] = 1;
  }
  freopen("terrain.mod", "w", stdout);
  cout << n << '\n';
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cout << ty[i][j] << " \n"[j == n];
    }
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      for (int k = 0; k < 3; k++) {
        cout << a[i][j][k] << " ";
      }
      cout << iss[i][j] << ' ';
      if (iss[i][j]) {
        cout << a[i][j][3] << ' ';
      }
      cout << '\n';
    }
  }
  fclose(stdout);
  return 0;
}
