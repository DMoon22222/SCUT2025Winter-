/**
 * @file shortest_path.cpp
 * @brief 使用迪杰斯特拉算法计算最小交换花费
 *
 * 该程序将交换问题建模为加权有向图，使用迪杰斯特拉算法
 * 计算从起始物品到目标物品的最小花费路径。
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <map>

using namespace std;

/**
 * @class Graph
 * @brief 使用邻接矩阵表示的加权有向图
 *
 * 该类实现了迪杰斯特拉算法，用于计算最短路径。
 * 支持泛型顶点类型，可适用于各种交换场景。
 */
template<typename T>
class Graph {
protected:
    int V;                          // 顶点数
    vector<vector<double>> adjMatrix;  // 邻接矩阵
    map<T, int> vertexIndex;        // 顶点到索引的映射
    map<int, T> indexVertex;        // 索引到顶点的映射

public:
    /**
     * @brief 构造函数
     *
     * @param vertices 顶点列表
     */
    Graph(const vector<T>& vertices) {
        V = vertices.size();
        adjMatrix.resize(V, vector<double>(V, numeric_limits<double>::infinity()));

        // 初始化顶点映射
        for (int i = 0; i < V; i++) {
            vertexIndex[vertices[i]] = i;
            indexVertex[i] = vertices[i];

            // 顶点到自身的距离为0
            adjMatrix[i][i] = 0;
        }
    }

    /**
     * @brief 添加有向边
     *
     * @param from 起始顶点
     * @param to 目标顶点
     * @param weight 边权重
     */
    void addEdge(T from, T to, double weight) {
        int u = vertexIndex[from];
        int v = vertexIndex[to];
        adjMatrix[u][v] = weight;
    }

    /**
     * @brief 迪杰斯特拉算法计算最短路径
     *
     * @param src 源顶点
     * @param dest 目标顶点
     * @return 包含路径和总花费的pair
     */
    pair<vector<T>, double> dijkstra(T src, T dest) {
        int start = vertexIndex[src];
        int end = vertexIndex[dest];

        vector<double> dist(V, numeric_limits<double>::infinity());
        vector<int> prev(V, -1);
        vector<bool> visited(V, false);

        dist[start] = 0;

        // 使用优先队列（最小堆）优化
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            double currentDist = pq.top().first;
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;

            // 提前终止条件
            if (u == end) break;

            for (int v = 0; v < V; v++) {
                if (adjMatrix[u][v] < numeric_limits<double>::infinity()) {
                    double newDist = dist[u] + adjMatrix[u][v];

                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        prev[v] = u;
                        pq.push({newDist, v});
                    }
                }
            }
        }

        // 构建路径
        vector<T> path;
        int current = end;

        if (dist[end] == numeric_limits<double>::infinity()) {
            return {path, -1};  // 不可达
        }

        while (current != -1) {
            path.insert(path.begin(), indexVertex[current]);
            current = prev[current];
        }

        return {path, dist[end]};
    }

    /**
     * @brief 打印图的邻接矩阵
     */
    void printGraph() {
        cout << "邻接矩阵:" << endl;
        cout << "    ";

        for (int i = 0; i < V; i++) {
            cout << indexVertex[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < V; i++) {
            cout << indexVertex[i] << " ";
            for (int j = 0; j < V; j++) {
                if (adjMatrix[i][j] == numeric_limits<double>::infinity()) {
                    cout << "INF ";
                } else {
                    cout << adjMatrix[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
};

/**
 * @class ExchangeProblem
 * @brief 特定交换问题的封装类
 *
 * 继承自Graph类，专门处理物品交换问题。
 */
class ExchangeProblem : public Graph<string> {
public:
    /**
     * @brief 构造函数
     *
     * 初始化特定的交换问题
     */
    ExchangeProblem() : Graph<string>({"乐谱", "海报", "黑胶", "鼓", "吉他", "古筝"}) {
        setupExchangeRules();
    }

private:
    /**
     * @brief 设置交换规则
     */
    void setupExchangeRules() {
        // 小光：海报 -> 乐谱，黑胶 -> 乐谱+50
        addEdge("海报", "乐谱", 0);
        addEdge("黑胶", "乐谱", 50);

        // 小红：黑胶 -> 鼓+150，海报 -> 鼓+300
        addEdge("黑胶", "鼓", 150);
        addEdge("海报", "鼓", 300);

        // 小李：黑胶 -> 吉他+200，海报 -> 吉他+350
        addEdge("黑胶", "吉他", 200);
        addEdge("海报", "吉他", 350);

        // 小范：鼓 -> 古筝+100，吉他 -> 古筝+200
        addEdge("鼓", "古筝", 100);
        addEdge("吉他", "古筝", 200);

        // 注意：交换是可逆的，但需要重新定义权重
        addEdge("乐谱", "海报", 0);  // 假设等价交换
        addEdge("乐谱", "黑胶", -50); // 需要对方加钱
    }

public:
    /**
     * @brief 计算最小花费交换路径
     *
     * @param from 起始物品
     * @param to 目标物品
     */
    void calculateBestExchange(string from, string to) {
        auto result = dijkstra(from, to);

        if (result.second < 0) {
            cout << "无法从 " << from << " 交换到 " << to << endl;
            return;
        }

        cout << "\n最佳交换路径 (" << from << " -> " << to << "):" << endl;
        cout << "=========================================" << endl;

        for (size_t i = 0; i < result.first.size(); i++) {
            cout << result.first[i];
            if (i < result.first.size() - 1) {
                cout << " -> ";
            }
        }

        cout << "\n总花费: " << result.second << " 元" << endl;
        cout << "=========================================" << endl;

        // 详细解释交换过程
        cout << "\n详细交换过程:" << endl;
        cout << "-----------------------" << endl;

        for (size_t i = 0; i < result.first.size() - 1; i++) {
            string fromItem = result.first[i];
            string toItem = result.first[i + 1];

            // 这里可以添加更详细的交换规则解释
            cout << "步骤" << i + 1 << ": 用" << fromItem;
            cout << " 交换 " << toItem;

            // 获取交换成本
            double cost = getCost(fromItem, toItem);
            if (cost > 0) {
                cout << "，需要额外支付 " << cost << " 元";
            } else if (cost < 0) {
                cout << "，对方需要额外支付 " << -cost << " 元";
            } else {
                cout << "，等价交换";
            }
            cout << endl;
        }
    }

private:
    /**
     * @brief 获取两个物品间的交换成本
     */
    double getCost(string from, string to) {
        int u = vertexIndex[from];
        int v = vertexIndex[to];
        return adjMatrix[u][v];
    }
};

/**
 * @brief 主函数
 *
 * 演示交换问题的求解
 */
int main() {
    cout << "=== 物品交换最小花费问题 ===" << endl;
    cout << "小明想用乐谱换古筝，求最小花费方案" << endl;

    // 创建交换问题实例
    ExchangeProblem exchange;

    // 打印交换图
    exchange.printGraph();

    // 计算最小花费路径
    exchange.calculateBestExchange("乐谱", "古筝");

    cout << endl;

    // 测试其他交换路径
    cout << "\n=== 其他交换方案测试 ===" << endl;
    exchange.calculateBestExchange("海报", "古筝");
    exchange.calculateBestExchange("黑胶", "古筝");

    return 0;
}
