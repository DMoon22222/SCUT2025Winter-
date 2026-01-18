#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;

class GraphMatrix {
private:
    int numVertices;
    int **matrix;

public:
    GraphMatrix(int n) {
        numVertices = n;
        matrix = new int *[n];
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[n];
            for (int j = 0; j < n; j++) {
                matrix[i][j] = 0;
            }
        }
    }

    ~GraphMatrix() {
        for (int i = 0; i < numVertices; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void addEdge(int u, int v, int w) {
        matrix[u][v] = w;
        matrix[v][u] = w;
    }

    void printMatrix() {
        cout << "Matrix before printing:" << endl;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                cout << setw(4) << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class GraphList {
    int numVertices;
    list<pair<int, int> > *adjList;

public:
    GraphList(int n) {
        numVertices = n;
        adjList = new list<pair<int, int> >[n];
    }

    ~GraphList() {
        delete[] adjList;
    }

    void addEdge(int u, int v, int w) {
        adjList[u].push_back(make_pair(v, w));
        adjList[v].push_back(make_pair(u, w));
    }

    void printList() {
        cout << "List after printing:" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << setw(4) << i << " ";
            for (auto neighbour: adjList[i]) {
                cout << "->(" << neighbour.first << "," << neighbour.second << ")";
            }
            cout << endl;
        }
    }
};

int main() {
    int V = 5;

    GraphMatrix gm(V);
    gm.addEdge(0, 1, 10);
    gm.addEdge(0, 4, 20);
    gm.addEdge(1, 2, 30);
    gm.printMatrix();

    GraphList gl(V);
    gl.addEdge(0, 1, 10);
    gl.addEdge(0, 4, 20);
    gl.addEdge(1, 2, 30);
    gl.printList();
}
