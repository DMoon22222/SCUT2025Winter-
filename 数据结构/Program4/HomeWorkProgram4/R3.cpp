/**
 * @file HomeWorkProgram4/R3.cpp
 * @brief 计算朋友圈数量的实现
 *
 * 该程序通过构建无向图并使用DFS识别连通分量来计算朋友圈数量。
 * 每个连通分量代表一个朋友圈。
 */

#include <iostream>
#include <vector>
#include <list>
#include <queue>

using namespace std;

/**
 * @class Graph
 * @brief 使用邻接表表示的无向图
 *
 * 该类使用向量存储邻接表，支持添加边和深度优先搜索遍历。
 */
class Graph {
private:
    int V;                       // 顶点数
    vector<list<int>> adj;       // 邻接表
    vector<bool> visited;        // 访问标记数组

    /**
     * @brief 深度优先搜索的递归实现
     *
     * @param v 当前访问的顶点
     * @param component 存储当前连通分量的向量
     */
    void DFSUtil(int v, vector<int>& component) {
        visited[v] = true;
        component.push_back(v);

        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                DFSUtil(neighbor, component);
            }
        }
    }

public:
    /**
     * @brief 构造函数
     *
     * @param vertices 顶点数
     */
    Graph(int vertices) : V(vertices), adj(vertices), visited(vertices, false) {}

    /**
     * @brief 添加无向边
     *
     * @param u 边的第一个顶点
     * @param v 边的第二个顶点
     */
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    /**
     * @brief 计算连通分量
     *
     * @return 包含所有连通分量的二维向量
     */
    vector<vector<int>> connectedComponents() {
        // 重置访问标记
        fill(visited.begin(), visited.end(), false);

        vector<vector<int>> components;

        for (int v = 0; v < V; v++) {
            if (!visited[v]) {
                vector<int> component;
                DFSUtil(v, component);
                components.push_back(component);
            }
        }

        return components;
    }
};

/**
 * @brief 打印朋友圈信息
 *
 * @param components 包含所有朋友圈的向量
 */
void printComponents(const vector<vector<int>>& components) {
    cout << "朋友圈总数: " << components.size() << endl;
    cout << "======================" << endl;

    for (size_t i = 0; i < components.size(); i++) {
        cout << "朋友圈 " << i + 1 << " ("
             << components[i].size() << "人): ";

        for (size_t j = 0; j < components[i].size(); j++) {
            cout << "学生" << components[i][j];
            if (j < components[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

/**
 * @brief 主函数
 *
 * 演示图连通分量算法的使用
 */
int main() {
    // 测试数据1: 简单示例
    cout << "=== 测试1: 简单示例 ===" << endl;
    int N1 = 6;
    Graph g1(N1);

    // 添加朋友关系: (0,1), (1,2), (3,4) 形成两个朋友圈
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(3, 4);

    vector<vector<int>> components1 = g1.connectedComponents();
    printComponents(components1);

    cout << endl;

    // 测试数据2: 复杂示例
    cout << "=== 测试2: 复杂示例 ===" << endl;
    int N2 = 10;
    Graph g2(N2);

    // 添加朋友关系
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    g2.addEdge(1, 2);
    g2.addEdge(1, 3);
    g2.addEdge(4, 5);
    g2.addEdge(6, 7);
    g2.addEdge(7, 8);
    g2.addEdge(8, 9);

    vector<vector<int>> components2 = g2.connectedComponents();
    printComponents(components2);

    return 0;
}