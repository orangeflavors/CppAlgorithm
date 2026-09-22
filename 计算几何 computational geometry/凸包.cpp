#include<bits/stdc++.h>
using namespace std;

const double eps = 1e-6;
int sgn(double x){
    if(fabs(x) < eps) return 0;
    else return x < 0 ? -1 : 1;
}

struct Point{
    double x, y;
    Point(){}
    Point(double x, double y):x(x), y(y){}
    Point operator + (Point B){return Point(x + B.x, y + B.y);}
    Point operator - (Point B){return Point(x - B.x, y - B.y);}
    bool operator == (Point B){return sgn(x - B.x) == 0 && sgn(y - B.y) == 0;}
    bool operator < (Point B){
        return sgn(x - B.x) < 0 || (sgn(x - B.x) == 0 && sgn(y - B.y) < 0);
    }
};
typedef Point Vector;
double Cross(Vector A, Vector B){return A.x * B.y - A.y * B.x;}
double Distance(Point A, Point B){return hypot(A.x - B.x, A.y - B.y);}

int Convex_hull(vector<Point> &p, int n, vector<Point> &ch){
    sort(p.begin(), p.end());
    int v = 0;
    for(int i = 0; i < n; i++){
        while(v > 1 && sgn(Cross(ch[v - 1] - ch[v - 2], p[i] - ch[v - 1])) <= 0){
            v--;
        }
        ch[v++] = p[i];
    }

    int j = v;
    for(int i = n - 2; i >= 0; i--){
        while(v > j && sgn(Cross(ch[v - 1] - ch[v - 2], p[i] - ch[v - 1])) <= 0){
            v--;
        }
        ch[v++] = p[i];
    }
    if(n > 1) v--;
    return v;
}

vector<Point> p, ch;// p 输入的点 ch 凸包上的点