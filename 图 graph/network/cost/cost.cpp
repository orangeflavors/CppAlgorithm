#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 5005;
const int M = 200000;
const ll INF = 1e18;

struct Edge
{
    int to;
    int next;
    int cap;
    ll cost;
};
Edge e[M];
int head[N];
int idx = 0;
void addEdge(int u, int v, int cap, ll cost)
{
    e[idx] = {v, head[u], cap, cost};
    head[u] = idx++;
    e[idx] = {u, head[v], 0, -cost};
    head[v] = idx++;
}

ll dis[N];
ll h[N]; // 势能
int pre[N];     // 前驱边

bool dijkstra(int s, int t, int n)
{
    fill(dis, dis + n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dis[s] = 0;
    pq.push({0, s});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dis[u])
            continue;
        for (int i = head[u]; i != -1; i = e[i].next)
        {
            auto &ed = e[i];
            if (ed.cap <= 0)
                continue;

            int v = ed.to;
            ll nd = d + ed.cost + h[u] - h[v];
            if (nd < dis[v])
            {
                dis[v] = nd;
                pre[v] = i;
                pq.push({nd, v});
            }
        }
    }

    return dis[t] != INF;
}

pair<int, ll> minCostMaxFlow(int s, int t, int n)
{
    int flow = 0;
    ll cost = 0;
    memset(h, 0, sizeof(h));
    while (dijkstra(s, t, n))
    {
        for (int i = 1; i <= n; i++)
            if (dis[i] != INF)
                h[i] += dis[i];
        int f = 1e9;
        for (int v = t; v != s;)
        {
            int id = pre[v];
            f = min(f, e[id].cap);
            v = e[id ^ 1].to;
        }

        for (int v = t; v != s;)
        {
            int id = pre[v];
            e[id].cap -= f;
            e[id ^ 1].cap += f;
            cost += 1LL * f * e[id].cost;
            v = e[id ^ 1].to;
        }

        flow += f;
    }

    return {flow, cost};
}

int main()
{
    memset(head, -1, sizeof(head));

    return 0;
}