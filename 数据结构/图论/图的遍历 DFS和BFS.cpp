#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include <iomanip>
using namespace std;

class Graph {
    int VertexCount;
    vector<vector<int> > adj;

public:
    Graph(int v) :VertexCount(v) ,adj(v){}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        //若为无向图，添加 adj[u].push_back(u);

    }

    void DFSUtil(int v, vector<bool> &visited) {
        visited[v] = true;
        cout << v << " ";
        for (auto neighbour: adj[v]) {
            if (!visited[neighbour]) {
                DFSUtil(neighbour, visited);
            }
        }
    }
    /*
     void DFSUtil(int v,vector<bool>&visited)
     {
     visited[v]=true;
     cout<<v<<" ";
     for(auto neighbour:adj[v])
     {
        if(!visited[neighbor])
        {
           DFSUtil(neighbour,visited);
        }
     }

     }
     void DFS(int startVertex)
     {
     vector<bool> visited(VertexCount,false);
     cout<<"DFS:from vertex "<<startVertex<<endl;
     DFSUtil(startVertex,visited);
     cout<<endl;
     }
     */

    void DFS(int startVertex) {
        vector<bool> visited(VertexCount, false);
        cout << "DFS:from vertex " << startVertex << endl;

        DFSUtil(startVertex, visited);
        cout << endl;
    }
    void BFS(int start)
    {
        vector<bool> visited(VertexCount,false);//标记顶点是否已访问
        queue<int> q;//BFS核心：队列储存待访问顶点
        //初始化：标记起点为已访问，加入队列
        visited[start] = true;
        q.push(start);
        while(!q.empty())
        {
            int current=q.front();//取出队首顶点，访问并输出
            q.pop();
            cout<<current<<" ";
            //遍历当前顶点的所有邻接顶点，未访问则标记加入队列
            for(int neighbor:adj[current])
            {
                if(!visited[neighbor])
                {
                    q.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
        cout<<endl;
    }
    /*
    void BFS(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);
        cout << "BFS:from vertex " << startVertex << "\n";
        while (!q.empty()) {
            int current = q.front();
            cout << current << " ";
            q.pop();

            for (int neighbour: adjList[current]) {
                if (!visited[neighbour]) {
                    q.push(neighbour);
                    visited[neighbour] = true;
                }
            }
        }
        cout << "\n";
    }
      */
};

int main() {
    Graph g(5); // 0, 1, 2, 3, 4
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3); // 自环

    // 运行遍历
    g.BFS(2); // 预期输出类似于: 2 0 3 1
    g.DFS(2); // 预期输出类似于: 2 0 1 3
}
