#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;
typedef pair<int, int> iPair;

class Graph {
    int numVertices;
    list<pair<int, int> > *adjList;

public:
    Graph(int V) {
        numVertices = V;
        adjList = new list<pair<int, int> >[V];
    }

    ~Graph() {
        delete[] adjList;
    }

    void addEdge(int u, int v, int w) {
        adjList[u].push_back(make_pair(v, w));
        adjList[v].push_back(make_pair(u, w));
    }

    void shortestPath(int startVertex) {
        priority_queue<iPair, vector<iPair>, greater<iPair> > pq;
        int *dist = new int[numVertices];
        for (int i = 0; i < numVertices; i++) {
            dist[i] = INT_MAX;
        }
        pq.push(make_pair(0, startVertex));
        dist[startVertex] = 0;
        cout << "Shortest path from :" << endl;
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            for (auto i: adjList[u]) {
                int v = i.first;
                int weight = i.second;

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        cout << "顶点\t距离" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << i << "\t\t" << (dist[i] == INT_MAX ? -1 : dist[i]) << endl;
        }
        delete[] dist;
    }
};

int main() {
    int V = 9;
    Graph g(V);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    g.shortestPath(0);
    return 0;
}
