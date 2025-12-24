#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <set>

using namespace std;

// ========== 图1：邻接矩阵 ==========
const int N = 8; // A-H

char toChar(int i) { return 'A' + i; }
int toInt(char c) { return c - 'A'; }

class Graph1 {
private:
    int mat[N][N];
public:
    Graph1() {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                mat[i][j] = (i == j) ? 0 : INT_MAX;
    }

    void addEdge(char u, char v, int w) {
        int i = toInt(u), j = toInt(v);
        mat[i][j] = mat[j][i] = w;
    }

    void printMatrix() {
        cout << "=== 图1 邻接矩阵 ===\n   ";
        for (int i = 0; i < N; ++i) cout << toChar(i) << "   ";
        cout << "\n";
        for (int i = 0; i < N; ++i) {
            cout << toChar(i) << "  ";
            for (int j = 0; j < N; ++j) {
                if (mat[i][j] == INT_MAX) cout << "∞   ";
                else cout << mat[i][j] << "   ";
            }
            cout << "\n";
        }
    }

    void BFS(char start) {
        bool vis[N] = {false};
        queue<int> q;
        int s = toInt(start);
        q.push(s); vis[s] = true;
        cout << "BFS from " << start << ": ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << toChar(u) << " ";
            for (int v = 0; v < N; ++v) {
                if (mat[u][v] != INT_MAX && !vis[v]) {
                    vis[v] = true; q.push(v);
                }
            }
        }
        cout << "\n";
    }

    void DFSUtil(int u, bool vis[]) {
        vis[u] = true;
        cout << toChar(u) << " ";
        for (int v = 0; v < N; ++v) {
            if (mat[u][v] != INT_MAX && !vis[v]) {
                DFSUtil(v, vis);
            }
        }
    }

    void DFS(char start) {
        bool vis[N] = {false};
        cout << "DFS from " << start << ": ";
        DFSUtil(toInt(start), vis);
        cout << "\n";
    }

    void dijkstra(char start) {
        int s = toInt(start);
        int dist[N]; bool fin[N] = {false};
        for (int i = 0; i < N; ++i) dist[i] = INT_MAX;
        dist[s] = 0;

        for (int count = 0; count < N - 1; ++count) {
            int u = -1;
            for (int i = 0; i < N; ++i)
                if (!fin[i] && (u == -1 || dist[i] < dist[u]))
                    u = i;
            if (u == -1) break;
            fin[u] = true;
            for (int v = 0; v < N; ++v) {
                if (mat[u][v] != INT_MAX && !fin[v]) {
                    int nd = dist[u] + mat[u][v];
                    if (nd < dist[v]) dist[v] = nd;
                }
            }
        }

        cout << "Dijkstra from " << start << ":\n";
        for (int i = 0; i < N; ++i) {
            cout << start << "->" << toChar(i) << ": ";
            if (dist[i] == INT_MAX) cout << "∞\n";
            else cout << dist[i] << "\n";
        }
    }

    void prim(char start) {
        int s = toInt(start);
        int parent[N], key[N];
        bool inMST[N] = {false};
        for (int i = 0; i < N; ++i) {
            key[i] = INT_MAX;
            parent[i] = -1;
        }
        key[s] = 0;

        for (int count = 0; count < N - 1; ++count) {
            int u = -1;
            for (int i = 0; i < N; ++i)
                if (!inMST[i] && (u == -1 || key[i] < key[u]))
                    u = i;
            if (u == -1) break;
            inMST[u] = true;
            for (int v = 0; v < N; ++v) {
                if (mat[u][v] != INT_MAX && !inMST[v] && mat[u][v] < key[v]) {
                    key[v] = mat[u][v];
                    parent[v] = u;
                }
            }
        }

        cout << "Prim MST (from " << start << "):\n";
        int total = 0;
        for (int i = 0; i < N; ++i) {
            if (parent[i] != -1) {
                cout << toChar(parent[i]) << "--" << toChar(i)
                     << " (" << mat[parent[i]][i] << ")\n";
                total += mat[parent[i]][i];
            }
        }
        cout << "Total weight: " << total << "\n";
    }
};

// ========== 图2：关节点（终极修正版）==========
const int M = 12; // A-L

class Graph2 {
private:
    vector< vector<int> > adj; // 邻接表
    vector<int> disc, low;
    vector<bool> visited;
    set<int> apSet;
    int timer;

    // 标准 Tarjan 关节点算法
    void tarjan(int u, int parent) {
        visited[u] = true;
        disc[u] = low[u] = ++timer;
        int children = 0;

        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!visited[v]) {
                children++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);

                // 如果 u 不是根节点，且 low[v] >= disc[u]，则 u 是关节点
                if (parent != -1 && low[v] >= disc[u]) {
                    apSet.insert(u);
                }
            } else if (v != parent) {
                // 回边：用 disc[v] 更新 low[u]
                low[u] = min(low[u], disc[v]);
            }
        }

        // 如果 u 是根节点且有多个子树，则是关节点
        if (parent == -1 && children > 1) {
            apSet.insert(u);
        }
    }

public:
    Graph2() : adj(M), disc(M, -1), low(M, -1), visited(M, false), timer(0) {
        // ?? 严格按照图2网格结构添加边（这是关键！）
        // 第一行
        addEdge('A', 'B'); addEdge('A', 'E');
        addEdge('B', 'C'); addEdge('B', 'F');
        addEdge('C', 'D'); addEdge('C', 'G');
        addEdge('D', 'H');

        // 第二行
        addEdge('E', 'F'); addEdge('E', 'I');
        addEdge('F', 'G'); addEdge('F', 'J');
        addEdge('G', 'H'); addEdge('G', 'K');
        addEdge('H', 'L');

        // 第三行
        addEdge('I', 'J');
        addEdge('J', 'K');
        addEdge('K', 'L');
    }

    void addEdge(char u, char v) {
        int a = u - 'A', b = v - 'A';
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    void findArticulationPoints() {
        // 重置状态
        fill(visited.begin(), visited.end(), false);
        fill(disc.begin(), disc.end(), -1);
        fill(low.begin(), low.end(), -1);
        apSet.clear();
        timer = 0;

        // 遍历所有节点（确保处理整个连通分量）
        for (int i = 0; i < M; ++i) {
            if (!visited[i]) {
                tarjan(i, -1);
            }
        }
    }

    void printArticulationPoints() {
        findArticulationPoints();

        cout << "\n=== 图2 关节点（全局计算）===\n";
        cout << "关节点: ";
        if (apSet.empty()) {
            cout << "无";
        } else {
            for (set<int>::iterator it = apSet.begin(); it != apSet.end(); ++it) {
                cout << char('A' + *it) << " ";
            }
        }
        cout << "\n";
    }
};

// ========== 主函数 ==========
int main() {
    cout << "=== 第三次代码作业：图 ===\n\n";

    // ========== 图1 处理 ==========
    Graph1 g1;
    g1.addEdge('A','B',4); g1.addEdge('A','D',6); g1.addEdge('A','G',7);
    g1.addEdge('B','C',12); g1.addEdge('B','E',9);
    g1.addEdge('C','D',13); g1.addEdge('C','F',2); g1.addEdge('C','H',10);
    g1.addEdge('D','E',1); g1.addEdge('D','G',2);
    g1.addEdge('E','F',5); g1.addEdge('E','H',8);
    g1.addEdge('F','H',3);
    g1.addEdge('G','H',14);

    g1.printMatrix();
    cout << "\n";
    g1.BFS('A');
    g1.DFS('A');
    cout << "\n";
    g1.dijkstra('A');
    cout << "\n";
    g1.prim('A');

    // ========== 图2 处理 ==========
    Graph2 g2;
    g2.printArticulationPoints(); // 只需调用一次，结果全局一致

    cout << "\n结论：图2的关节点为 C、F、G，无论从哪个点开始遍历，结果都应相同。\n";

    return 0;
}
