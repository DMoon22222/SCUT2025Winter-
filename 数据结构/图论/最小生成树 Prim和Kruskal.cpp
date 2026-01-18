#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;
typedef pair<int, int> iPair;

struct Edge {
    int src, dest, weight;
};

bool compareEdges(Edge a, Edge b) {
    return a.weight < b.weight;
}

struct DSU {
    int *parent;

    DSU(int n) {
        parent = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    ~DSU() {
        delete[] parent;
    }

    int find(int i) {
        if (parent[i] == i) {
            return i;
        }
        return find(parent[i]);
    }

    void union1(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j)
            parent[root_i] = root_j;
    }
};

void KruskalMST(vector<Edge>& edges, int V) {
    sort(edges.begin(), edges.end(), compareEdges);

    DSU dsu(V);
    int cost = 0;

    cout<<"---Kruskal---"<<endl;
    for (Edge e : edges) {
        int u = e.src;
        int v = e.dest;
        if (dsu.find(u) != dsu.find(v)) {
            dsu.union1(u, v);
            cost += e.weight;
            cout <<"选中边："<<u<<"——"<<v<<"（权重"<<e.weight<<"）"<<endl;
        }
    }
    cout<<"Kruskal MST总权重="<<cost<<endl;
}

void PrimMST(list<iPair>* adj, int V) {
    priority_queue<iPair,vector<iPair>,greater<iPair>> pq;
    int src=0;
    int* key=new int[V];
    int* parent=new int[V];
    bool* inMST=new bool[V];

    for (int i = 0; i < V; i++) {
        key[i]=1e9;
        parent[i]=-1;
        inMST[i]=false;
    }

    pq.push(make_pair(0,src));
    key[src]=0;
    int cost=0;
    cout<<"---Prim---"<<endl;
    while (!pq.empty()) {
        int u=pq.top().second;
        pq.pop();
        if (inMST[u]) {
            continue;
        }
        inMST[u]=true;
        if (parent[u]!=-1) {
            cout<<"选中连接："<<parent[u]<<"——"<<u<<"（权重"<<key[u]<<"）"<<endl;
            cost+=key[u];
        }

        for (auto x : adj[u]) {
            int v=x.first;
            int weight=x.second;

            if (inMST[v]==false&&weight<key[v]) {
                key[v]=weight;
                pq.push(make_pair(key[v],v));
                parent[v]=u;
            }
        }
    }
    cout<<"Prim MST总权重="<<cost<<endl;
    delete[] parent;
    delete[] key;
    delete[] inMST;
}
int main() {
    int V = 4;
    vector<Edge> edges = {
        {0, 1, 10}, {0, 2, 6}, {0, 3, 5},
        {1, 3, 15}, {2, 3, 4}
    };

    // 为 Prim 构建传统的邻接表数组
    list<iPair>* adj = new list<iPair>[V];
    for(auto e : edges) {
        adj[e.src].push_back({e.dest, e.weight});
        adj[e.dest].push_back({e.src, e.weight});
    }

    KruskalMST(edges, V);
    PrimMST(adj, V);

    delete[] adj; // 释放内存
    return 0;
}
