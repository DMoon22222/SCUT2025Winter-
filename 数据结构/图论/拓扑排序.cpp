#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;

class Graph {
    int numVertices;
    list<int> *adjList;

public:
    Graph(int V) {
        numVertices = V;
        adjList = new list<int>[V];
    }

    ~Graph() {
        delete[] adjList;
    }

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
    }

    void topologicalSort() {
        int *inDegree = new int[numVertices];
        for (int i = 0; i < numVertices; i++) {
            inDegree[i] = 0;
        }

        for (int i = 0; i < numVertices; i++) {
            for (auto neighbour: adjList[i]) {
                inDegree[neighbour]++;
            }
        }

        queue<int> q;
        for (int i = 0; i < numVertices; i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }

        int count = 0;
        vector<int> topOrder;
        while (!q.empty()) {
            int top = q.front();
            q.pop();
            topOrder.push_back(top);
            count++;
            for (auto neighbour: adjList[top]) {
                inDegree[neighbour]--;
                if (inDegree[neighbour] == 0) {
                    q.push(neighbour);
                }
            }
        }

        if (count != numVertices) {
            cout << "有环，无法进行拓扑排序" << endl;
        } else {
            cout << "topological sorted" << endl;
            for (int i: topOrder) {
                cout << i << " ";
            }
            cout << endl;
        }
        delete[] inDegree;
    }
};

int main() {
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    cout << "任务依赖图构建完成，开始排序..." << endl;
    g.topologicalSort();
    return 0;
}
