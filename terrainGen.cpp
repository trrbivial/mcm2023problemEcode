#include <bits/stdc++.h>
#define db double
#define ll long long
using namespace std;

#define ROAD 10
#define RIVER 20
#define PLAIN 30
#define FOREST 40
#define BUILDING 50
#define HOUSE 60
#define MOUNTAIN 70

const ll seed = 19817;
// const ll seed = 19260817;

// must be odd
int n = 21;

const int N = 20005;
const int M = 2005;

struct Template {
  int ty;
  db reflectRatio;
  db absorbRatio;
  db declineRatio;
  int isLightSource;
  db intensity;
  Template(int type = PLAIN, db ref = 0.0, db abs = 0.0, db dec = 0.0,
           int iss = 0, db li = 0.0) {
    ty = type;
    reflectRatio = ref;
    absorbRatio = abs;
    declineRatio = dec;
    isLightSource = iss;
    intensity = li;
  }
} tempBlk[N], mmp[M][M];

const db proRoad = 0.98;
const db proRiver = 0.7;

db genDouble(db l, db r) {
  return l + static_cast<db>(rand()) / (static_cast<db>(RAND_MAX / (r - l)));
}
ll genInt(ll l, ll r) { return l + rand() % (r - l + 1); }

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
ll hei[M][M];
void accessRoad(int x, int y, int fx, int fy, int ty, int width = 0) {
  while (true) {
    for (int i = -width; i <= width; i++) {
      for (int j = -width; j <= width; j++) {
        if (x + i < 1 || x + i > n || y + j < 1 || y + j > n)
          continue;
        if ((ll)i * i + (ll)j * j <= (ll)width * width) {
          if (ty == RIVER) {
            if (hei[x + i][y + j] <= 1) {
              mmp[x + i][y + j] = tempBlk[ty];
              hei[x + i][y + j] = 0;
            }
            if (hei[x + i][y + j] == 2 && genInt(1, 3) == 1) {
              mmp[x + i][y + j] = tempBlk[ty];
              hei[x + i][y + j] = 0;
            }
          } else if (ty == ROAD) {
            if (hei[x + i][y + i] <= 1) {
              mmp[x + i][y + j] = tempBlk[ty];
              hei[x + i][y + j] = 1;
            }
          }
        }
      }
    }
    if (fx == x && fy == y)
      return;

    if (genDouble(0, 1) <= (ty == ROAD ? proRoad : proRiver)) {
      int cnt = 0;
      int rx[5], ry[5];
      db prob[5];
      for (int i = 0; i < 4; i++) {
        int lx = x + dx[i], ly = y + dy[i];
        if (abs(lx - fx) + abs(ly - fy) < abs(x - fx) + abs(y - fy)) {
          rx[++cnt] = lx;
          ry[cnt] = ly;
          if (abs(lx - fx) < abs(x - fx)) {
            prob[cnt] = 1.0 * abs(x - fx) / (abs(x - fx) + abs(y - fy));
          }
          if (abs(ly - fy) < abs(y - fy)) {
            prob[cnt] = 1.0 * abs(y - fy) / (abs(x - fx) + abs(y - fy));
          }
        }
      }
      assert(cnt > 0);
      if (cnt == 1) {
        x = rx[1];
        y = ry[1];
      } else if (cnt == 2) {
        db r = genDouble(0, 1);
        if (r <= prob[1]) {
          x = rx[1];
          y = ry[1];
        } else {
          x = rx[2];
          y = ry[2];
        }
      }
    } else {
      int cnt = 0;
      int rx[5], ry[5];
      for (int i = 0; i < 4; i++) {
        int lx = x + dx[i], ly = y + dy[i];
        if (lx < 1 || lx > n || ly < 1 || ly > n)
          continue;
        rx[++cnt] = dx[i];
        ry[cnt] = dy[i];
      }
      assert(cnt > 0);
      int r = genInt(1, cnt);
      x = x + rx[r], y = y + ry[r];
    }
  }
}

void genHeightMap(int n) {

  hei[0][0] = genDouble(0, 256);
  hei[n - 1][0] = genDouble(0, 256);
  hei[0][n - 1] = genDouble(0, 256);
  hei[n - 1][n - 1] = genDouble(0, 256);

  queue<tuple<int, int, int, int, ll>> q;
  q.push(make_tuple(0, 0, n - 1, n - 1, 200));

  while (q.size()) {
    auto [top, left, bottom, right, randomness] = q.front();
    q.pop();
    if (right < left)
      continue;
    if (bottom < top)
      continue;
    int centerX = (left + right) / 2;
    int centerY = (top + bottom) / 2;

    hei[centerX][top] = (hei[left][top] + hei[right][top]) / 2 +
                        genInt(-randomness, randomness);
    hei[centerX][bottom] = (hei[left][bottom] + hei[right][bottom]) / 2 +
                           genInt(-randomness, randomness);
    hei[left][centerY] = (hei[left][top] + hei[left][bottom]) / 2 +
                         genInt(-randomness, randomness);
    hei[right][centerY] = (hei[right][top] + hei[right][bottom]) / 2 +
                          genInt(-randomness, randomness);

    hei[centerX][centerY] = (hei[left][top] + hei[right][top] +
                             hei[left][bottom] + hei[right][bottom]) /
                                4 +
                            genInt(-randomness, randomness);

    if (right - left > 2 || bottom - top > 2) {
      q.push(make_tuple(top, left, centerY, centerX, randomness / 2));
      q.push(make_tuple(top, centerX, centerY, right, randomness / 2));
      q.push(make_tuple(centerY, left, bottom, centerX, randomness / 2));
      q.push(make_tuple(centerY, centerX, bottom, right, randomness / 2));
    }
  }
}

const int riverNum = 2;
void genRiver() {
  int t[5] = {0, 0, 0, 0, 0};
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      t[hei[i][j]]++;
    }
  }
  int realNum = riverNum;
  if (min(t[3] + t[2], t[0]) < riverNum) {
    realNum = min(t[3] + t[2], t[0]);
  }
  vector<pair<int, int>> st;
  vector<pair<int, int>> ed;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (hei[i][j] >= 2) {
        st.push_back(make_pair(i, j));
      }
      if (hei[i][j] <= 0) {
        ed.push_back(make_pair(i, j));
      }
    }
  }
  random_shuffle(st.begin(), st.end());
  random_shuffle(ed.begin(), ed.end());
  for (int i = 0; i < realNum; i++) {
    auto [x, y] = st[i];
    auto [fx, fy] = ed[i];
    accessRoad(x, y, fx, fy, RIVER);
  }
}

const db sig1 = 1.0 / 6;
const db sig2 = 1.0 / 6;
const db pi = acos(-1);
const db rho = 0.8;

db normDistribution(db x, db y) {
  x = x / n;
  y = y / n;
  db ret = exp(-0.5 * (x / sig1 * x / sig1 + y / sig2 * y / sig2)) /
           (2 * pi * sig1 * sig2);
  return rho * ret;
}
void genTempCity(std::function<db(db, db)> func) {
  int cp = (n + 1) >> 1;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      int x = i - cp, y = j - cp;
      if (genDouble(0, 1) <= func(x, y)) {
        int ty = genInt(1, 4);
        if (ty <= 3)
          ty += BUILDING;
        else
          ty = HOUSE;
        if (hei[i][j] == 3 && genInt(1, 5) == 0) {
          mmp[i][j] = tempBlk[ty];
        } else if (hei[i][j] == 2 && genInt(1, 3) == 0) {
          mmp[i][j] = tempBlk[ty];
        } else if (hei[i][j] == 1 && genInt(1, 50) <= 49) {
          mmp[i][j] = tempBlk[ty];
        }
      }
    }
  }
}

#define pii pair<int, int>

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

#define mp make_pair

void initdir() {
  set<pii> tmp;
  tmp.insert(mp(-1, 0));
  tmp.insert(mp(1, 0));
  tmp.insert(mp(0, -1));
  tmp.insert(mp(0, 1));
  tmp.insert(mp(1732, -1000));
  tmp.insert(mp(1732, 1000));
  tmp.insert(mp(-1732, -1000));
  tmp.insert(mp(-1732, 1000));
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

pii findStep(int dx, int dy, int st) {
  auto &&vec = vecDirectionOrder[mp(dx, dy)];
  int idx = (n + 1) / 2, idy = (n + 1) / 2;
  for (auto &&[dir, ref] : vec) {
    auto [tx, ty] = dir;
    int lx = idx + tx, ly = idy + ty;
    if ((--st) == 0)
      return mp(lx, ly);
    if (tx == 0 && ty == 0)
      continue;
    if (lx < 1 || lx > n || ly < 1 || ly > n)
      return mp(max(min(lx, n), 1), max(min(ly, n), 1));
  }
  return mp(0, 0);
}

void genStringCity() {
  genTempCity(normDistribution);
  int len = n / 2;
  {
    auto [lx, ly] = findStep(1732, -1000, n);
    auto [rx, ry] = findStep(-1732, 1000, n);
    accessRoad(lx, ly, rx, ry, ROAD);
  }
  {
    auto [lx, ly] = findStep(-1732, -1000, n);
    auto [rx, ry] = findStep(1732, 1000, n);
    accessRoad(lx, ly, rx, ry, ROAD);
  }
  {
    auto [lx, ly] = findStep(0, -1, n);
    auto [rx, ry] = findStep(0, 1, n);
    accessRoad(lx, ly, rx, ry, ROAD);
  }
  {
    int x[] = {1732, 0, -1732, -1732, 0, 1732};
    int y[] = {-1000, -1, -1000, 1000, 1, 1000};
    int l[] = {len, n / 3, len, len, n / 3, len};
    for (int i = 0; i < 6; i++) {
      {
        auto [lx, ly] = findStep(x[i], y[i], l[i] / 2);
        auto [rx, ry] =
            findStep(x[(i + 1) % 6], y[(i + 1) % 6], l[(i + 1) % 6] / 2);
        accessRoad(lx, ly, rx, ry, ROAD);
      }
      {
        auto [lx, ly] = findStep(x[i], y[i], l[i]);
        auto [rx, ry] =
            findStep(x[(i + 1) % 6], y[(i + 1) % 6], l[(i + 1) % 6]);
        accessRoad(lx, ly, rx, ry, ROAD);
      }
    }
  }
}

const int splitNum = 5;
const db sig3 = 1.0 / 8;
const db rho3 = 1;
db shiftDistribution(db x, db y) {
  x = x / n;
  y = y / n;
  db ret =
      exp(-0.5 * (max(x * x, y * y) / sig3 / sig3)) / (2 * pi * sig3 * sig3);
  return rho3 * ret;
}
void genBlockCity() {
  genTempCity(shiftDistribution);
  int m = n * sig3 * 2 * 2;
  int d = m / splitNum;
  if (splitNum % 2 == 1) {
    for (int i = -(splitNum / 2); i <= (splitNum / 2); i++) {
      {
        auto [lx, ly] = findStep(-1, 0, n / 3);
        auto [rx, ry] = findStep(1, 0, n / 3);
        accessRoad(lx, ly + i * d, rx, ry + i * d, ROAD);
      }
      {
        auto [lx, ly] = findStep(0, -1, n / 3);
        auto [rx, ry] = findStep(0, 1, n / 3);
        accessRoad(lx + i * d, ly, rx + i * d, ry, ROAD);
      }
    }
  } else {
    for (int i = -(splitNum / 2); i <= (splitNum / 2); i++) {
      if (i == 0)
        continue;
      if (i < 0) {

      } else {
      }
    }
  }
}

void randomGen() {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      mmp[i][j] = tempBlk[PLAIN];
      hei[i][j] = 1;
    }
  }
  genHeightMap(n);
  ll mx = -(1 << 30), mi = (1 << 30);
  for (int i = n; i >= 1; i--) {
    for (int j = n; j >= 1; j--) {
      hei[i][j] = hei[i - 1][j - 1];
      mx = max(mx, hei[i][j]);
      mi = min(mi, hei[i][j]);
    }
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      db tmp = 1.0 * (hei[i][j] - mi) / (mx - mi + 1);
      if (tmp > 0.77) {
        hei[i][j] = 3;
        mmp[i][j] = tempBlk[MOUNTAIN];
      } else if (tmp > 0.73) {
        hei[i][j] = 2;
        mmp[i][j] = tempBlk[FOREST];
      } else if (tmp < 0.2) {
        hei[i][j] = 0;
        mmp[i][j] = tempBlk[RIVER];
      } else {
        hei[i][j] = 1;
      }
    }
  }
  genRiver();

  initdir();

  // genStringCity();
  //  genBlockCity();
}

void genBeijingCity(std::function<db(db, db)> func) {
  int cp = (n + 1) >> 1;
  vector<pair<int, int>> id;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      id.push_back(make_pair(i, j));
    }
  }
  random_shuffle(id.begin(), id.end());
  for (auto [i, j] : id) {
    int x = i - cp, y = j - cp;
    int ty = genInt(1, 3);
    if (ty <= 2)
      ty += BUILDING;
    else
      ty = HOUSE;
    int r = genInt(1, 50);
    if (mmp[i][j].ty == PLAIN) {
      if (r <= 49) {
        int sz = genInt(0, 2);
        for (int k = -sz; k <= sz; k++) {
          for (int l = -sz; l <= sz; l++) {
            if (i + k < 1 || i + k > n || j + l < 1 || j + l > n)
              continue;
            if (mmp[i + k][j + l].ty == PLAIN) {
              mmp[i + k][j + l] = tempBlk[ty];
            }
          }
        }
      }
    }
  }
}
void genBeijing() {
  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.2, 1, 10);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.2, 1, 30);
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.2, 1, 100);
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 30);
tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
*/

  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8, 1, 5);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.2, 1, 10);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.2, 1, 5);  // 4
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.2, 1, 98); // 95
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 5);                // 4
tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
*/

  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.3, 1, 6);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.2, 1, 20);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.2, 1, 30);
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.2, 1, 30);
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 30);
tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
*/

  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.5, 1, 10);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.5);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.5);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.3);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.1, 1, 10);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.02, 0.03, 0.1, 1, 30); // 1
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.1, 1, 100);
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.05, 1, 30); // 1
                                                          */

  freopen("beijing4.in", "r", stdin);
  cin >> n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      int ty;
      cin >> ty;
      mmp[n - j + 1][i] = tempBlk[ty];
    }
  }
  fclose(stdin);
  n--;
  genBeijingCity(shiftDistribution);
}
void genCountryside() {
  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.15, 1, 10);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.15, 1, 5);
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.15, 1, 100);
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 4.5);
tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
*/

  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.15, 1, 10);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.15, 1, 5);
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.15, 1, 100);
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 5);
tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
*/

  /*
tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8);
tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.15, 1, 10);
tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.15, 1, 5);
tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.15, 1, 100);
tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.57, 1, 5);
tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
*/

  tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8);
  tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
  tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
  tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
  tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.2, 1, 10);
  tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.2, 1, 5);
  tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.2, 1, 100);
  tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 5);
  tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
  freopen("countryside3.in", "r", stdin);
  cin >> n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      int ty;
      cin >> ty;
      mmp[n - j + 1][i] = tempBlk[ty];
      /*
      if (ty == HOUSE) {
        if (genInt(1, 10) == 1) {
          mmp[n - j + 1][i].intensity *= 0.8;
        }
      }
      */
    }
  }
  fclose(stdin);
  n--;
}

void genYellowstoneForest(std::function<db(db, db)> func) {
  int cp = (n + 1) >> 1;
  vector<pair<int, int>> id;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      id.push_back(make_pair(i, j));
    }
  }
  random_shuffle(id.begin(), id.end());
  for (auto [i, j] : id) {
    int ty = genInt(1, 6);
    int sz = genInt(0, 2);
    if (ty == 1) {
      ty = PLAIN;
    } else if (ty <= 3) {
      ty = FOREST;
      sz += 1;
    } else if (ty <= 6) {
      ty = MOUNTAIN;
      sz += 2;
    }
    if (mmp[i][j].ty == BUILDING) {

      for (int k = -sz; k <= sz; k++) {
        for (int l = -sz; l <= sz; l++) {
          if (i + k < 1 || i + k > n || j + l < 1 || j + l > n)
            continue;
          if (mmp[i + k][j + l].ty == BUILDING) {
            mmp[i + k][j + l] = tempBlk[ty];
          }
        }
      }
    }
  }
}
void genYellowstone() {
  freopen("yellowstone3.in", "r", stdin);
  cin >> n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      int ty;
      cin >> ty;
      mmp[n - j + 1][i] = tempBlk[ty];
      if (ty == FOREST) {
        mmp[n - j + 1][i].ty = BUILDING;
      }
      if (ty == ROAD) {
        mmp[n - j + 1][i].isLightSource = 0;
      }
    }
  }
  fclose(stdin);
  genYellowstoneForest(shiftDistribution);
}

void genCountyPlain(std::function<db(db, db)> func) {
  int cp = (n + 1) >> 1;
  vector<pair<int, int>> id;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      id.push_back(make_pair(i, j));
    }
  }
  random_shuffle(id.begin(), id.end());
  for (auto [i, j] : id) {
    int ty = genInt(1, 100);
    int sz = genInt(0, 1);
    if (ty <= 49) {
      ty = PLAIN;
    } else if (ty <= 99) {
      ty = FOREST;

    } else {
      ty = HOUSE;
      sz += 1;
    }
    if (mmp[i][j].ty == 0) {

      for (int k = -sz; k <= sz; k++) {
        for (int l = -sz; l <= sz; l++) {
          if (i + k < 1 || i + k > n || j + l < 1 || j + l > n)
            continue;
          if (mmp[i + k][j + l].ty == 0) {
            mmp[i + k][j + l] = tempBlk[ty];
          }
          if (mmp[i + k][j + l].ty == PLAIN && ty == HOUSE) {
            mmp[i + k][j + l] = tempBlk[ty];
          }
        }
      }
    }
  }
}
void genCounty() {
  tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.95, 1, 2);
  tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.95);
  tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.95);
  tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.9);
  tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.2, 1, 10);
  tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.2, 1, 5);
  tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.2, 1, 100);
  tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.85, 1, 5);
  tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);
  freopen("county3.in", "r", stdin);
  cin >> n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      int ty;
      cin >> ty;
      mmp[n - j + 1][i] = tempBlk[ty];
      if (ty == 0) {
        mmp[n - j + 1][i].ty = 0;
      }
    }
  }
  fclose(stdin);
  genCountyPlain(shiftDistribution);
}
void init() {
  tempBlk[ROAD] = Template(ROAD, 0.0, 0.2, 0.8, 1, 10);
  tempBlk[RIVER] = Template(RIVER, 0.0, 0.1, 0.8);
  tempBlk[PLAIN] = Template(PLAIN, 0.0, 0.2, 0.8);
  tempBlk[FOREST] = Template(FOREST, 0.01, 0.3, 0.6);
  tempBlk[BUILDING + 0] = Template(BUILDING + 0, 0.02, 0.3, 0.2, 1, 10);
  tempBlk[BUILDING + 1] = Template(BUILDING + 1, 0.5, 0.03, 0.2, 1, 5);
  tempBlk[BUILDING + 2] = Template(BUILDING + 2, 0.02, 0.3, 0.2, 1, 100);
  tempBlk[HOUSE] = Template(HOUSE, 0.03, 0.3, 0.6, 1, 5);
  tempBlk[MOUNTAIN] = Template(MOUNTAIN, 0.01, 0.4, 0.05);

  // randomGen();

  // genBeijing();
  genCountryside();
  // genYellowstone();
  // genCounty();
}

int main() {
  srand(seed);
  init();

  freopen("terrain.in", "w", stdout);
  cout << n << '\n';
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cout << mmp[i][j].ty << " \n"[j == n];
    }
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cout << mmp[i][j].reflectRatio << ' ' << mmp[i][j].absorbRatio << ' '
           << mmp[i][j].declineRatio << ' ' << mmp[i][j].isLightSource << ' ';
      if (mmp[i][j].isLightSource)
        cout << mmp[i][j].intensity << ' ';
      cout << '\n';
    }
  }

  return 0;
}
