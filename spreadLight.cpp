#include <bits/stdc++.h>
#define db double
#define ll long long
#define pii pair<int, int>
#define mp make_pair
#define mt make_tuple
using namespace std;

// must be odd

int n = 201;
int cp;

const int N = 2005;
const db distancePerblock = 20;
struct Point {
  int idx, idy;
  int x, y;
  db x_d, y_d;

  Point(int lx = 0, int ly = 0) {
    idx = lx;
    idy = ly;
    x = lx - cp;
    y = ly - cp;
    x_d = (lx - cp) * distancePerblock;
    y_d = (ly - cp) * distancePerblock;
  }
} block[N][N];

pair<int, int> getIndex(db rx, db ry) {
  return mp(llround(rx / distancePerblock + cp),
            llround(ry / distancePerblock + cp));
}

pair<int, int> getCoord(int idx, int idy) { return mp((idx - cp), (idy - cp)); }
pair<db, db> getRealCoord(int idx, int idy) {
  return mp((idx - cp) * distancePerblock, (idy - cp) * distancePerblock);
}

#define ROAD 10
#define RIVER 20
#define PLAIN 30
#define FOREST 40
#define BUILDING 50
#define HOUSE 60
#define MOUNTAIN 70

int ty[N][N];

db reflectRatio[N][N];
db absorbRatio[N][N];
db declineRatio[N][N];

struct lightInfo {
  db intensity;
  lightInfo(db _intensity = 0.0) { intensity = _intensity; }
} blockLightInfo[N][N];

map<pii, lightInfo> blockLightGiveOut[N][N];

const ll disrange = 4;

vector<pii> vecInRange;
map<pii, vector<pair<pii, pii>>> vecDirectionOrder;

int isCrossX(int i, int j, int dx, int dy) {
  if (dy == 0)
    return false;
  db k1 = (j - 0.5) / dy, k2 = (j + 0.5) / dy;
  db px1 = k1 * dx, px2 = k2 * dx;
  return (i - 0.5 <= px1 && px1 <= i + 0.5) +
         (i - 0.5 <= px2 && px2 <= i + 0.5);
}
int isCrossY(int i, int j, int dx, int dy) {
  if (dx == 0)
    return false;
  db k1 = (i - 0.5) / dx, k2 = (i + 0.5) / dx;
  db py1 = k1 * dy, py2 = k2 * dy;
  return (j - 0.5 <= py1 && py1 <= j + 0.5) +
         (j - 0.5 <= py2 && py2 <= j + 0.5);
}
db calcCrossX(int i, int j, int dx, int dy) {
  db k1 = (j - 0.5) / dy, k2 = (j + 0.5) / dy;
  db px1 = k1 * dx, px2 = k2 * dx;
  if (i - 0.5 <= px1 && px1 <= i + 0.5)
    return k1;
  if (i - 0.5 <= px2 && px2 <= i + 0.5)
    return k2;
  assert(false);
}
db calcCrossY(int i, int j, int dx, int dy) {
  db k1 = (i - 0.5) / dx, k2 = (i + 0.5) / dx;
  db py1 = k1 * dy, py2 = k2 * dy;
  if (j - 0.5 <= py1 && py1 <= j + 0.5)
    return k1;
  if (j - 0.5 <= py2 && py2 <= j + 0.5)
    return k2;

  assert(false);
}

void init() {
  cp = ((n + 1) >> 1);
  set<pii> tmp;
  for (ll i = -disrange; i <= disrange; i++) {
    for (ll j = -disrange; j <= disrange; j++) {
      if (i == 0 && j == 0)
        continue;
      if (i * i + j * j <= disrange * disrange) {

        tmp.insert(mp(i, j));
      }
    }
  }
  for (auto [dx, dy] : tmp) {
    bool flg = true;
    for (auto &&[lx, ly] : vecInRange) {
      if (dx * ly - dy * lx == 0 && dx * lx + dy * ly >= 0) {
        flg = false;
        break;
      }
    }
    if (flg)
      vecInRange.push_back(mp(dx, dy));
  }
  int maxN = n * 2 + 10;
  for (auto &&[dx, dy] : vecInRange) {
    set<pair<db, pair<pii, pii>>> s;
    db norm2 = (ll)dx * dx + (ll)dy * dy;
    for (ll i = -maxN; i <= maxN; i++) {
      for (ll j = -maxN; j <= maxN; j++) {
        ll area = abs(i * dy - j * dx);
        if (2 * area * area < norm2) {
          db sdw = (i * dx + j * dy) / sqrt(norm2);
          if (sdw <= 0)
            continue;
          int rx, ry;
          int cntx = isCrossX(i, j, dx, dy);
          int cnty = isCrossY(i, j, dx, dy);
          if (cntx == 0 && cnty == 0)
            continue;
          if (cntx == 2 && cnty == 2) {
            rx = -dx;
            ry = -dy;
          } else if (cntx == 2) {
            rx = dx;
            ry = -dy;
          } else if (cnty == 2) {
            rx = -dx;
            ry = dy;
          } else {
            assert(cntx == 1 && cnty == 1);
            db kx = calcCrossX(i, j, dx, dy), ky = calcCrossY(i, j, dx, dy);
            if (kx < ky) {
              rx = dx;
              ry = -dy;
            } else {
              rx = -dx;
              ry = dy;
            }
          }
          s.insert(mp(sdw, mp(mp(i, j), mp(rx, ry))));
        }
      }
    }

    vector<pair<pii, pii>> tmp;
    for (auto [_, p] : s) {
      tmp.push_back(p);
    }
    vecDirectionOrder[mp(dx, dy)] = tmp;
  }
}

bool isLightSource[N][N];

const db eps = 1e-2;

map<pii, db> accumuIntensity[N][N];

db sumIntensity[N][N];

const int maxIterTime = 10;

map<pii, db> accumuIntensityTmp[N][N];
void spreadLight() {
  for (int cs = 0; cs <= maxIterTime; cs++) {
    queue<tuple<pii, pii, lightInfo>> q;
    if (cs == 0) {
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {

          if (isLightSource[i][j]) {
            sumIntensity[i][j] += blockLightInfo[i][j].intensity;
            for (auto &&[dx, dy] : vecInRange) {
              q.push(mt(mp(i, j), mp(dx, dy), blockLightInfo[i][j]));
            }
          }
        }
      }
    } else {
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
          if (!accumuIntensity[i][j].empty()) {
            for (auto &&[dir, li] : accumuIntensity[i][j]) {
              auto [dx, dy] = dir;
              q.push(mt(mp(i, j), mp(dx, dy), li));
            }
          }
        }
      }
    }
    while (q.size()) {
      auto [ta, tb, li] = q.front();
      auto [idx, idy] = ta;
      auto [dx, dy] = tb;
      q.pop();
      if (li.intensity < eps)
        continue;
      auto &&vec = vecDirectionOrder[mp(dx, dy)];
      for (auto &&[dir, ref] : vec) {
        auto [tx, ty] = dir;
        int lx = idx + tx, ly = idy + ty;
        if (tx == 0 && ty == 0)
          continue;
        if (lx < 1 || lx > n || ly < 1 || ly > n)
          break;
        db refIntensity = li.intensity * reflectRatio[lx][ly];
        if (refIntensity >= eps) {
          if (accumuIntensityTmp[lx][ly].find(ref) !=
              accumuIntensityTmp[lx][ly].end()) {
            accumuIntensityTmp[lx][ly][ref] += refIntensity;
          } else {
            accumuIntensityTmp[lx][ly][ref] = refIntensity;
          }
        }
        db absIntensity = li.intensity * absorbRatio[lx][ly];
        sumIntensity[lx][ly] += absIntensity;

        db crsIntensity = li.intensity * declineRatio[lx][ly];
        if (crsIntensity < eps) {
          break;
        }
        li.intensity = crsIntensity;
      }
    }
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        swap(accumuIntensity[i][j], accumuIntensityTmp[i][j]);
        accumuIntensityTmp[i][j].clear();
      }
    }
  }
}

int main() {
  freopen("terrain.mod", "r", stdin);
  cin >> n;
  init();
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cin >> ty[i][j];
    }
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cin >> reflectRatio[i][j] >> absorbRatio[i][j] >> declineRatio[i][j];
      cin >> isLightSource[i][j];
      if (isLightSource[i][j])
        cin >> blockLightInfo[i][j].intensity;
    }
  }

  spreadLight();

  int su = 0, tt = 0;
  db sum = 0;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (BUILDING <= ty[i][j] && ty[i][j] <= HOUSE) {
        su++;
        if (sumIntensity[i][j] > 1199) {
          tt++;
        }
        sum += sumIntensity[i][j];
      }
    }
  }
  freopen("disturb.txt", "w", stdout);
  cout << sum / su << '\n';
  fclose(stdout);
  freopen("lightmap.out", "w", stdout);
  cout << n << '\n';
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cout << sumIntensity[i][j] << " \n"[j == n];
    }
  }
  fclose(stdout);

  return 0;
}
