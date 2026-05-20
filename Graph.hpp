#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

//Adjacency list
class Graph {
    private:
        std::vector<std::vector<int>> adj;
        int n;
    public:
        Graph(int n = 0) : n{n}, adj(n, std::vector<int>(n)) {}
        Graph(const Graph& other) : n{other.n}, adj{other.adj} {}
        Graph(Graph&& other) noexcept : n{std::move(other.n)}, adj{std::move(other.adj)} {other.n = 0; }
        ~Graph() = default;

        Graph& operator=(const Graph& other);
        Graph& operator=(Graph&& other) noexcept;

        void addEdge(int u, int v, bool isUndirected = true);
        
        std::vector<int> DFS();
        void DFS(int u, std::vector<bool>& visited, std::vector<int>& res);
        std::vector<int> BFS();

        bool pathExistsDFS(int u, int v);
        bool pathExistsDFS(int u, int v, std::vector<bool>& visited);
        bool pathExistsBFS(int u, int v);

        int shortestPathDFS(int u, int v);
        void shortestPathDFS(int u, int v, std::vector<bool>& visited, int& res, int tmp);        
        int shortestPathBFS(int u, int v);

        int getNumberOfComponentsDFS();
        void DFS_helper(int u, std::vector<bool>& visited);
        int getNumberOfComponentsBFS();

        std::vector<std::vector<int>> getAllPathsDFS(int u, int v);
        void getAllPathsDFS(int u, int v, std::vector<bool>& visited, std::vector<int>& tmp, std::vector<std::vector<int>>& res);
        std::vector<std::vector<int>> getAllPathsBFS(int u, int v);

        void transpose();
        bool has_cycle(bool isUndirected = true);
        bool has_cycleUndirected(std::vector<bool>& visited, int s, int p);
        bool has_cycleDirected(std::vector<int>& visited, int s);

        int getLevelCountDFS(int s, int level);
        void getLevelCountDFS(int s, int level, std::vector<bool>& visited, int& count);
        int getLevelCountBFS(int s, int level);
};

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        n = other.n;
        adj = other.adj;
    }
    return *this;
}

Graph& Graph::operator=(Graph&& other) noexcept {
    if (this != &other) {
        n = std::move(other.n);
        adj = std::move(other.adj);
        other.n = 0;
    }
    return *this;
}

void Graph::addEdge(int u, int v, bool isUndirected) {
    adj[u].push_back(v);
    if (isUndirected) adj[v].push_back(u);
}

std::vector<int> Graph::DFS() {
    std::vector<int> res {};
    std::vector<bool> visited(n);
    for (int i{}; i < n; ++i) {
        if (!visited[i]) DFS(i, visited, res);
    }
    return res;
}

void Graph::DFS(int u, std::vector<bool>& visited, std::vector<int>& res) {
    visited[u] = true;
    res.push_back(u);

    for (int nb : adj[u]) {
        if (!visited[nb]) DFS(nb, visited, res);
    }
}

std::vector<int> Graph::BFS() {
    std::vector<bool> visited(n);
    std::vector<int> res;
    std::queue<int> q;

    for (int i{}; i < n; ++i) {
        if (!visited[i]) {
    
            visited[i] = true;
            q.push(i);

            while (!q.empty()) {
                int curr = q.front();
                q.pop();

                res.push_back(curr);

                for (int nb : adj[curr]) {
                    if (!visited[nb]) { 
                        visited[nb] = true;
                        q.push(nb);
                    }
                }
            }
        }
    } 
    return res; 
}

bool Graph::pathExistsDFS(int u, int v) {
    std::vector<bool> visited(n);
    return pathExistsDFS(u, v, visited);
}

bool Graph::pathExistsDFS(int u, int v, std::vector<bool>& visited) {
    if (u == v) return true;

    visited[u] = true;
    for (int nb : adj[u]) {
        if (!visited[nb] && pathExistsDFS(nb, v, visited)) return true;
    }
    return false;
}

bool Graph::pathExistsBFS(int u, int v) {
    if (u == v) return true;
    
    std::vector<bool> visited(n);
    std::queue<int> q;
    q.push(u);
    visited[u] = true;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int nb : adj[curr]) {
            if (nb == v) return true;
            if (!visited[nb]) { 
                visited[nb] = true;
                q.push(nb);
            }
        }
    }
    return false;
}

int Graph::shortestPathDFS(int u, int v) {
    std::vector<bool> visited(n);
    int res = std::numeric_limits<int>::max();
    shortestPathDFS(u, v, visited, res, 0);
    return (res == std::numeric_limits<int>::max()) ? -1 : res; 
}

void Graph::shortestPathDFS(int u, int v, std::vector<bool>& visited, int& res, int tmp) {
    if (u == v) { 
        res = std::min(res, tmp);
        return;
    }

    visited[u] = true;
    for (int nb : adj[u]) {
        if (!visited[nb]) shortestPathDFS(nb, v, visited, res, tmp + 1);
    }
    visited[u] = false;
}   
    
int Graph::shortestPathBFS(int u, int v) {
    if (u == v) return 0;
    
    std::vector<bool> visited(n);
    std::queue<int> q;
    q.push(u);
    visited[u] = true;
    int level = 0;

    while (!q.empty()) {
        int s = q.size();
        while (s--) {
            int curr = q.front();
            q.pop();

            for (int nb : adj[curr]) {
                if (nb == v) return level + 1;
                if (!visited[nb]) { 
                    visited[nb] = true;
                    q.push(nb);
                }
            }
        }
        ++level;
    }
    return -1;
}

int Graph::getNumberOfComponentsDFS() {
    std::vector<bool> visited(n);
    int components = 0;

    for (int i{}; i < n; ++i) {
        if (!visited[i]) { 
            ++components;
            DFS_helper(i, visited);
        }
    }
    return components;
}

void Graph::DFS_helper(int u, std::vector<bool>& visited) {
    visited[u] = true;

    for (int nb : adj[u]) {
        if (!visited[nb]) DFS_helper(nb, visited);
    }
}

int Graph::getNumberOfComponentsBFS() {
    std::vector<bool> visited(n);
    std::queue<int> q;
    int components = 0;

    for (int i{}; i < n; ++i) {
        if (!visited[i]) { 
            ++components;
             
            visited[i] = true;
            q.push(i);

            while (!q.empty()) {
                int curr = q.front();
                q.pop();


                for (int nb : adj[curr]) {
                    if (!visited[nb]) { 
                        visited[nb] = true;
                        q.push(nb);
                    }
                }
            }
        }
    }
    return components;
}

std::vector<std::vector<int>> Graph::getAllPathsDFS(int u, int v) {
    std::vector<std::vector<int>> res {};
    std::vector<int> tmp;
    std::vector<bool> visited(n);

    getAllPathsDFS(u, v, visited, tmp, res);
    return res;
}

void Graph::getAllPathsDFS(int u, int v, std::vector<bool>& visited, std::vector<int>& tmp, std::vector<std::vector<int>>& res) {
    visited[u] = true;
    tmp.push_back(u);

    if (u == v) res.push_back(tmp);
    else {
        for (int nb : adj[u]) {
            if (!visited[nb]) getAllPathsDFS(nb, v, visited, tmp, res);
        }
    }

    visited[u] = false;
    tmp.pop_back();
} 

std::vector<std::vector<int>> Graph::getAllPathsBFS(int u, int v) {
    if (u == v) return {{u}};
    
    std::vector<std::vector<int>> res {};    
    std::queue<std::vector<int>> q;  
    q.push({u});

    while (!q.empty()) {        
        std::vector<int> tmp = q.front();
        q.pop();
        int curr = tmp.back();

        if (curr == v) {
            res.push_back(tmp);
        } else {
            for (int nb : adj[curr]) {
                if (std::find(tmp.begin(), tmp.end(), nb) == tmp.end()) {
                    std::vector<int> vec = tmp;
                    vec.push_back(nb);
                    q.push(vec);
                }
            }
        }
    }
    return res;
}

void Graph::transpose() {
    std::vector<std::vector<int>> new_adj(n, std::vector<int>{});
    for (int i{}; i < n; ++i) {
        for (int nb : adj[i]) {
            new_adj[nb].push_back(i);
        }
    }
    adj = std::move(new_adj);
}

bool Graph::has_cycle(bool isUndirected) {
    if (isUndirected) {
        std::vector<bool> visited(n);
        for (int i{}; i < n; ++i) {
            if (!visited[i] && has_cycleUndirected(visited, i, -1)) return true;
        } 
    } else {
        std::vector<int> vec(n);
        for (int i{}; i < n; ++i) {
            if (!vec[i] && has_cycleDirected(vec, i)) return true;
        } 
    }
    return false;
}

bool Graph::has_cycleUndirected(std::vector<bool>& visited, int s, int p) {
    visited[s] = true;
    for (int nb : adj[s]) {
        if (!visited[nb]) { 
            if(has_cycleUndirected(visited, nb, s)) return true;
        }
        else if (nb != p) return true;
    }
    return false;
}

bool Graph::has_cycleDirected(std::vector<int>& vec, int s) {
    vec[s] = 1;
    for (int nb : adj[s]) {
        if (!vec[nb]) { 
            if(has_cycleDirected(vec, nb)) return true;
        }
        else if (vec[nb] == 1) return true; 
    }

    vec[s] = 2;
    return false;
}

int Graph::getLevelCountBFS(int u, int level) {    
    std::vector<bool> visited(n);
    std::queue<int> q;
    q.push(u);

    visited[u] = true;

    while (!q.empty()) {
        int s = q.size();
        if (level == 0) return s;
        while (s--) {
            int curr = q.front();
            q.pop();

            for (int nb : adj[curr]) {
                if (!visited[nb]) { 
                    visited[nb] = true;
                    q.push(nb);
                }
            }
        }
        --level;
    }
    return -1;
}

//Adjacency matrix
// class Graph {
//     private:
//         std::vector<std::vector<int>> adj;
//         int n;
//     public:
//         Graph(int n = 0) : n{n}, adj(n, std::vector<int>(n)) {}
//         Graph(const Graph& other) : n{other.n}, adj{other.adj} {}
//         Graph(Graph&& other) noexcept : n{std::move(other.n)}, adj{std::move(other.adj)} {other.n = 0; }
//         ~Graph() = default;

//         Graph& operator=(const Graph& other);
//         Graph& operator=(Graph&& other) noexcept;

//         void addEdge(int u, int v, bool isUndirected = true);
        
//         std::vector<int> DFS();
//         void DFS(int u, std::vector<bool>& visited, std::vector<int>& res);
//         std::vector<int> BFS();

//         bool pathExistsDFS(int u, int v);
//         bool pathExistsDFS(int u, int v, std::vector<bool>& visited);
//         bool pathExistsBFS(int u, int v);

//         int shortestPathDFS(int u, int v);
//         void shortestPathDFS(int u, int v, std::vector<bool>& visited, int& res, int tmp);        
//         int shortestPathBFS(int u, int v);

//         int getNumberOfComponentsDFS();
//         void DFS_helper(int u, std::vector<bool>& visited);
//         int getNumberOfComponentsBFS();
//         void BFS_helper(int u, std::vector<bool>& visited);

//         std::vector<std::vector<int>> getAllPathsDFS(int u, int v);
//         void getAllPathsDFS(int u, int v, std::vector<bool>& visited, std::vector<int>& tmp, std::vector<std::vector<int>>& res);
//         std::vector<std::vector<int>> getAllPathsBFS(int u, int v);

//         void transpose();
//         bool has_cycle(bool isUndirected = true);
//         bool has_cycleUndirected(std::vector<bool>& visited, int s, int p);
//         bool has_cycleDirected(std::vector<int>& visited, int s);

//         int getLevelCountBFS(int u, int level);    
// };

// Graph& Graph::operator=(const Graph& other) {
//     if (this != &other) {
//         n = other.n;
//         adj = other.adj;
//     }
//     return *this;
// }

// Graph& Graph::operator=(Graph&& other) noexcept {
//     if (this != &other) {
//         n = std::move(other.n);
//         adj = std::move(other.adj);
//         other.n = 0;
//     }
//     return *this;
// }

// void Graph::addEdge(int u, int v, bool isUndirected) {
//     adj[u][v] = 1;
//     if (isUndirected) adj[v][u] = 1;
// }

// std::vector<int> Graph::DFS() {
//     std::vector<int> res {};
//     std::vector<bool> visited(n);

//     for (int i{}; i < n; ++i) {
//         if (!visited[i]) DFS(i, visited, res);
//     }
//     return res;
// }

// void Graph::DFS(int u, std::vector<bool>& visited, std::vector<int>& res) {
//     visited[u] = true;
//     res.push_back(u);

//     for (int nb{}; nb < n ; ++nb) {
//         if (adj[u][nb] && !visited[nb]) DFS(nb, visited, res);
//     }
// }

// std::vector<int> Graph::BFS() {
//     std::vector<bool> visited(n);
//     std::vector<int> res;
//     std::queue<int> q;

//     for (int i{}; i < n; ++i) {
//         if (!visited[i]) {
    
//             visited[i] = true;
//             q.push(i);

//             while (!q.empty()) {
//                 int curr = q.front();
//                 q.pop();

//                 res.push_back(curr);

//                 for (int nb{}; nb < n; ++nb) {
//                     if (adj[curr][nb] && !visited[nb]) { 
//                         visited[nb] = true;
//                         q.push(nb);
//                     }
//                 }
//             }
//         }
//     } 
//     return res;
// }

// bool Graph::pathExistsDFS(int u, int v) {
//     std::vector<bool> visited(n);
//     return pathExistsDFS(u, v, visited);
// }

// bool Graph::pathExistsDFS(int u, int v, std::vector<bool>& visited) {
//     if (u == v) return true;

//     visited[u] = true;
//     for (int nb{}; nb < n; ++nb) {
//         if (adj[u][nb] && !visited[nb] && pathExistsDFS(nb, v, visited)) return true;
//     }
//     return false;
// }

// bool Graph::pathExistsBFS(int u, int v) {
//     if (u == v) return true;
    
//     std::vector<bool> visited(n);
//     std::queue<int> q;

//     q.push(u);
//     visited[u] = true;

//     while (!q.empty()) {
//         int curr = q.front();
//         q.pop();

//         for (int nb{}; nb < n; ++nb) {
//             if (adj[curr][nb]) {
//                 if (nb == v) return true;
//                 if (!visited[nb]) { 
//                     visited[nb] = true;
//                     q.push(nb);
//                 }
//             }
//         }
//     }
//     return false;
// }

// int Graph::shortestPathDFS(int u, int v) {
//     std::vector<bool> visited(n);
//     int res = std::numeric_limits<int>::max();

//     shortestPathDFS(u, v, visited, res, 0);
//     return (res == std::numeric_limits<int>::max()) ? -1 : res; 
// }

// void Graph::shortestPathDFS(int u, int v, std::vector<bool>& visited, int& res, int tmp) {
//     if (u == v) { 
//         res = std::min(res, tmp);
//         return;
//     }

//     visited[u] = true;
//     for (int nb{}; nb < n; ++nb) {
//         if (adj[u][nb] && !visited[nb]) shortestPathDFS(nb, v, visited, res, tmp + 1);
//     }
//     visited[u] = false;
// }   
    
// int Graph::shortestPathBFS(int u, int v) {
//     if (u == v) return 0;
    
//     std::vector<bool> visited(n);
//     std::queue<int> q;

//     q.push(u);
//     visited[u] = true;
//     int level = 0;

//     while (!q.empty()) {
//         int s = q.size();
//         while (s--) {
//             int curr = q.front();
//             q.pop();

//             for (int nb{}; nb < n; ++nb) {
//                 if (adj[curr][nb]) {
//                     if (nb == v) return level + 1;
//                     if (!visited[nb]) { 
//                         visited[nb] = true;
//                         q.push(nb);
//                     }
//                 }
//             }
//         }
//         ++level;
//     }
//     return -1;
// }

// int Graph::getNumberOfComponentsDFS() {
//     std::vector<bool> visited(n);
//     int components = 0;

//     for (int i{}; i < n; ++i) {
//         if (!visited[i]) { 
//             ++components;
//             DFS_helper(i, visited);
//         }
//     }
//     return components;
// }

// void Graph::DFS_helper(int u, std::vector<bool>& visited) {
//     visited[u] = true;

//     for (int nb{}; nb < n; ++nb) {
//         if (adj[u][nb] && !visited[nb]) DFS_helper(nb, visited);
//     }
// }

// int Graph::getNumberOfComponentsBFS() {
//     std::vector<bool> visited(n);
//     int components = 0;

//     for (int i{}; i < n; ++i) {
//         if (!visited[i]) { 
//             ++components;
//             BFS_helper(i, visited);
//         }
//     }
//     return components;
// }

// void Graph::BFS_helper(int s, std::vector<bool>& visited) {
//     visited[s] = true;
//     std::queue<int> q;
//     q.push(s);

//     while (!q.empty()) {
//         int curr = q.front();
//         q.pop();


//         for (int nb{}; nb < n; ++nb) {
//             if (adj[curr][nb] && !visited[nb]) { 
//                 visited[nb] = true;
//                 q.push(nb);
//             }
//         }
//     }
// }

// std::vector<std::vector<int>> Graph::getAllPathsDFS(int u, int v) {
//     std::vector<std::vector<int>> res {};
//     std::vector<int> tmp;
//     std::vector<bool> visited(n);

//     getAllPathsDFS(u, v, visited, tmp, res);
//     return res;
// }

// std::vector<std::vector<int>> Graph::getAllPathsBFS(int u, int v) {
//     if (u == v) return {{u}};
    
//     std::vector<std::vector<int>> res {};    
//     std::queue<std::vector<int>> q;  
//     q.push({u});

//     while (!q.empty()) {        
//         std::vector<int> tmp = q.front();
//         q.pop();
//         int curr = tmp.back();

//         if (curr == v) {
//             res.push_back(tmp);
//         } else {
//             for (int nb{}; nb < n; ++nb) {
//                 if (adj[curr][nb] && std::find(tmp.begin(), tmp.end(), nb) == tmp.end()) {
//                     std::vector<int> vec = tmp;
//                     vec.push_back(nb);
//                     q.push(vec);
//                 }
//             }
//         }
//     }
//     return res;
// }

// void Graph::getAllPathsDFS(int u, int v, std::vector<bool>& visited, std::vector<int>& tmp, std::vector<std::vector<int>>& res) {
//     visited[u] = true;
//     tmp.push_back(u);

//     if (u == v) res.push_back(tmp);
//     else {
//         for (int nb{}; nb < n; ++nb) {
//             if (adj[u][nb] && !visited[nb]) getAllPathsDFS(nb, v, visited, tmp, res);
//         }
//     }

//     visited[u] = false;
//     tmp.pop_back();
// } 


// void Graph::transpose() {
//     for (int i{}; i < n - 1; ++i) {
//         for (int j{i + 1}; j < n; ++j) {
//             std::swap(adj[i][j], adj[j][i]);
//         }
//     }
// }

// bool Graph::has_cycle(bool isUndirected) {
//     if (isUndirected) {
//         std::vector<bool> visited(n);
//         for (int i{}; i < n; ++i) {
//             if (!visited[i] && has_cycleUndirected(visited, i, -1)) return true;
//         } 
//     } else {
//         std::vector<int> vec(n);
//         for (int i{}; i < n; ++i) {
//             if (!vec[i] && has_cycleDirected(vec, i)) return true;
//         } 
//     }
//     return false;
// }

// bool Graph::has_cycleUndirected(std::vector<bool>& visited, int s, int p) {
//     visited[s] = true;
//     for (int nb{}; nb < n; ++nb) {
//         if (adj[s][nb]) {
//             if (!visited[nb]) { 
//                 if(has_cycleUndirected(visited, nb, s)) return true;
//             }
//             else if (nb != p) return true;
//         }
//     }
//     return false;
// }

// bool Graph::has_cycleDirected(std::vector<int>& vec, int s) {
//     vec[s] = 1;
//     for (int nb{}; nb < n; ++nb) {
//         if (adj[s][nb]) {
//             if (!vec[nb]) { 
//                 if(has_cycleDirected(vec, nb)) return true;
//             }
//             else if (vec[nb] == 1) return true; 
//         }
//     }

//     vec[s] = 2;
//     return false;
// }

// int Graph::getLevelCountBFS(int u, int level) {    
//     std::vector<bool> visited(n);
//     std::queue<int> q;
//     q.push(u);

//     visited[u] = true;

//     while (!q.empty()) {
//         int s = q.size();
//         if (level == 0) return s;
//         while (s--) {
//             int curr = q.front();
//             q.pop();

//             for (int nb{}; nb < n; ++nb) {
//                 if (adj[curr][nb] && !visited[nb]) { 
//                     visited[nb] = true;
//                     q.push(nb);
//                 }
//             }
//         }
//         --level;
    // }
    // return -1;
// }

#endif