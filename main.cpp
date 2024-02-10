#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>

using namespace std;
using ld = long double;
const ld INF = 1e9;
const ld deg = 3;
const ld PI = acos(-1);

void forStart() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
// geometry*************************************************************************************************************

struct Point {
    ld x, y;

    Point() {}

    Point(ld x1, ld y1) : x(x1), y(y1) {
    }

    Point operator+(Point a) const {
        return {x + a.x, y + a.y};
    }

    Point operator-(Point a) const {
        return {x - a.x, y - a.y};
    }

    ld operator*(Point a) const {
        return x * a.x + y * a.y;
    }

    ld operator%(Point a) const {
        return x * a.y - y * a.x;
    }

    ld len() const {
        return sqrt(x * x + y * y);
    }

    void rotate(ld angle) {
        ld newX = x * cos(angle) - y * sin(angle);
        ld newY = x * sin(angle) + y * cos(angle);
        x = newX;
        y = newY;
    }
};

struct Line {
    ld a, b, c;

    Line() {
    }

    Line(Point x, Point y) {
        a = x.y - y.y;
        b = y.x - x.x;
        c = x.x * y.y - y.x * x.y;
    }

    Point intersect(Line l) const {
        Point ans(-ld(c * l.b - b * l.c) / (a * l.b - l.a * b), -ld(a * l.c - l.a * c) / (a * l.b - l.a * b));
        return ans;
    }
};

bool onDifSide(Point a, Point b, Point c, Point d) {
    Point vec = b - a, vec1 = c - a, vec2 = d - a;
    if ((vec % vec1) * (vec % vec2) <= 0) {
        return true;
    }
    return false;
}

struct Ray {
    Point p, Vec;
    Line e;

    Ray() {}

    Ray(Point a1, Point Vec1) : p(a1), Vec(Vec1) {
        Vec.x *= 1e4;
        Vec.y *= 1e4;
        e = Line(p, Vec + p);
    }

    Point inters(Point c, Point d) {
        if (onDifSide(p, p + Vec, c, d) && onDifSide(c, d, p, p + Vec)) {
            Line a(c, d);
            return e.intersect(a);
        }
        return {-INF, -INF};
    }
};

int indx;

ld minDistance(const vector <pair<Point, Point>> &edge, Ray &r) {
    ld mn = INF;
    int i = indx = 0;
    for (auto [a, b]: edge) {
        Point p = r.inters(a, b);
        if (p.x != -INF && (p - r.p).len() < mn)
            mn = (p - r.p).len(), indx = i;
        i++;
    }
    i = 0;
    for (auto [a, b]: edge) {
        Point p = r.inters(a, b);
        if (p.x != -INF && abs((p - r.p).len() - mn) <= 1e-7 && indx != i) {
            indx = -1;
            break;
        }
        i++;
    }
    return mn;
}

// geometry*************************************************************************************************************
const int W = 284, H = 65;
char screen[H][W];
vector <pair<Point, Point>> edges;
#define sky 0
#define vall 1
#define floor 2
int timer = 0;

void output() {
    printf("\x1b[%dA", H + 2);
    for (int i = 0; i < H; ++i) {
        for (int j = i == 0; j < W; ++j) {
            switch (screen[i][j]) {
                case sky:
                    printf(" ");
                    break;
                case 3:
                    printf("\u2593");
                    break;
                case 4:
                    printf("\u2591");
                    break;
                case vall:
                    printf("\u2588");
                    break;
                case 69:
                    printf("|");
                    break;
                case floor:
                    if (i % 2 == j % 2 && timer % 2 || i % 2 != j % 2 && timer % 2 == 0) {
                        printf("o");
                    } else {
                        printf(".");
                    }
                    break;
                case '#':
                    printf("#");
                    break;
                case '.':
                    printf(" ");
                    break;
                case '@':
                    printf("@");
                    break;
            }
        }
        printf("\n");
    }
}

int32_t main() {
    ifstream fin("Input.txt");
    int n, m;
    fin >> n >> m;
    vector <string> map(n + 1);
    for (int i = 0; i < n; ++i) {
        fin >> map[i];
        map[i] += '*';
    }
    for (int i = 0; i < m; ++i) {
        map[n] += '*';
    }
    forStart();
    for (int i = 0; i < n; ++i) {
        int x = -1;
        for (int j = 0; j < m; ++j) {
            if (map[i][j] == '#') {
                if (x == -1) x = j;
                if (map[i][j] != map[i][j + 1]) {
                    Point a1((i + 1) * 10, (x + 1) * 10),
                            a2((i + 1) * 10, (j + 1) * 10);
                    edges.emplace_back(a1, a2);
//
//                    Point a3((i + 1) * 10 - 3, (x + 1) * 10),
//                            a4((i + 1) * 10 - 3, (j + 1) * 10);
//                    edges.emplace_back(a3, a4);
//
//                    Point a5((i + 1) * 10, (x + 1) * 10),
//                            a6((i + 1) * 10 - 3, (x + 1) * 10);
//                    edges.emplace_back(a5, a6);
//
//                    Point a7((i + 1) * 10, (j + 1) * 10),
//                            a8((i + 1) * 10 - 3, (j + 1) * 10);
//                    edges.emplace_back(a7, a8);
                    x = -1;
                }
            }
        }
    }
    for (int i = 0; i < m; ++i) {
        int x = -1;
        for (int j = 0; j < n; ++j) {
            if (map[j][i] == '#') {
                if (x == -1) x = j;
                if (map[j][i] != map[j + 1][i]) {
                    Point a1((x + 1) * 10, (i + 1) * 10),
                            a2((j + 1) * 10, (i + 1) * 10);
                    edges.emplace_back(a1, a2);
//
//                    Point a3((x + 1) * 10, (i + 1) * 10 - 3),
//                            a4((j + 1) * 10, (i + 1) * 10 - 3);
//                    edges.emplace_back(a3, a4);
//
//                    Point a5((x + 1) * 10, (i + 1) * 10),
//                            a6((x + 1) * 10, (i + 1) * 10 - 3);
//                    edges.emplace_back(a5, a6);
//
//                    Point a7((j + 1) * 10, (i + 1) * 10),
//                            a8((j + 1) * 10, (i + 1) * 10 - 3);
//                    edges.emplace_back(a7, a8);
                    x = -1;
                }
            }
        }
    }
    Ray person;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (map[i][j] == '@') {
                map[i][j] = '.';
                person = Ray(Point((i + 1) * 10, (j + 1) * 10), Point(-1, 0));
                break;
            }
        }
    }
    Ray move;
    move.p = person.p, move.Vec = Point(-1, 0);
    output();


    // для красоты

    vector <ld> kof(W);
    ld yg = -45 * PI / 180;
    ld step = (ld(100) / W) * PI / 180, st2 =  (ld(90) / W) * PI / 180;
    for (int i = 0; i < W; ++i) {
        kof[i] = cos(yg);
        yg += st2;
    }
    while (true) {
        char c = getchar();
        if (c == 'w' || c == 'W') {
            if (minDistance(edges, person) > 6.3) {
                move.p = move.p + move.Vec;
                person.p = move.p;
                timer++;
            }
        } else if (c == 'a' || c == 'A') {//left
            move.Vec.rotate(-deg * PI / 180);
            person.Vec.rotate(-deg * PI / 180);
            person.e = Line(person.p, person.p + person.Vec);
            timer++;
        } else if (c == 'd' || c == 'D') {// right
            move.Vec.rotate(deg * PI / 180);
            person.Vec.rotate(deg * PI / 180);
            person.e = Line(person.p, person.p + person.Vec);
            timer++;
        } else if (c == 's' || c == 'S') {
            person.Vec.x *= -1, person.Vec.y *= -1;
            if (minDistance(edges, person) > 6.3) {
                move.p = move.p - move.Vec;
                person.p = move.p;
                timer++;
            }
            person.Vec.x *= -1, person.Vec.y *= -1;
        }
        Ray a = person;
        a.Vec.rotate(-50 * PI / 180);
        a.e = Line(a.p, a.p + a.Vec);

        ld dist = minDistance(edges, a);
        dist = max(dist, ld(1));
        int col_cl = min(round(6 * H / (dist * kof[0])), ld(H));
        for (int j = 0; j < (H - col_cl + 1) / 2; ++j) {
            screen[j][0] = 0;
        }
        for (int j = (H - col_cl + 1) / 2; j < (H - col_cl + 1) / 2 + col_cl; ++j) {
            if (col_cl >= 10) {
                screen[j][0] = 1;
            } else if (col_cl >= 5) {
                screen[j][0] = 3;
            } else {
                screen[j][0] = 4;
            }
        }
        for (int j = (H - col_cl + 1) / 2 + col_cl; j < H; ++j) {
            screen[j][0] = 2;
        }

        for (int i = 0; i < W - 1; i++) {
            // code
            a.Vec.rotate(step);
            a.e = Line(a.p, a.p + a.Vec);
            ld dist = minDistance(edges, a), pr;
            int g = indx;
            dist = max(dist, ld(1));
            pr = dist;
            int col_cl = min(round(6 * H / (dist * kof[i + 1])), ld(H));
            for (int j = 0; j < (H - col_cl + 1) / 2; ++j) {
                screen[j][i + 1] = 0;
            }
            for (int j = (H - col_cl + 1) / 2; j < (H - col_cl + 1) / 2 + col_cl; ++j) {
                if (col_cl >= 10) {
                    screen[j][i + 1] = 1;
                } else if (col_cl >= 5) {
                    screen[j][i + 1] = 3;
                } else {
                    screen[j][i + 1] = 4;
                }
            }
            for (int j = (H - col_cl + 1) / 2 + col_cl; j < H; ++j) {
                screen[j][i + 1] = 2;
            }
            a.Vec.rotate(-step);
            a.e = Line(a.p, a.p + a.Vec);
            dist = minDistance(edges, a);
            dist = max(dist, ld(1));
            if (g != indx || indx == -1) {
                if (pr < dist) {
                    col_cl = min(round(6 * H / (pr * kof[i + 1])), ld(H));
                    for (int j = (H - col_cl + 1) / 2; j < (H - col_cl + 1) / 2 + col_cl; ++j) {
                        screen[j][i + 1] = 69;
                    }
                } else {
                    col_cl = min(round(6 * H / (dist * kof[i])), ld(H));
                    for (int j = (H - col_cl + 1) / 2; j < (H - col_cl + 1) / 2 + col_cl; ++j) {
                        screen[j][i] = 69;
                    }
                }
            }
            a.Vec.rotate(step);
            a.e = Line(a.p, a.p + a.Vec);
        }
        int x = move.p.x / 10, y = move.p.y / 10;
        for (int i = 0; i < n; ++i) {
            for (int j = W - m; j < W; ++j) {
                if (x - 1 != i || y - 1 != j - (W - m))
                    screen[i][j] = map[i][j - (W - m)];
                else {
                    screen[i][j] = '@';
                }
            }
        }
        output();
    }
}
