#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <limits>

class Solution {
    public:
    //Tarjan
        std::vector<std::vector<int>> tj(std::vector<std::vector<int>>& adj) {
            int time {};
            std::vector<int> low(adj.size(), -1);
            std::vector<int> disc(adj.size(), -1);
            std::vector<bool> onStack(adj.size());
            std::stack<int> st;
            std::vector<std::vector<int>> res;

            for (int i{}; i < adj.size(); ++i) {
                if (disc[i] == -1) tj(adj, i, low, disc, onStack, st, res, time);
            }
            return res;
        }  

        void tj( std::vector<std::vector<int>>& adj ,int u, std::vector<int>& low, 
std::vector<int>& disc, std::vector<bool>& onStack, std::stack<int>& st,  std::vector<std::vector<int>>& res, int& time) {
            disc[u] = low[u] = time++; 
            onStack[u] = true;
            st.push(u);

            for (int nb : adj[u]) {
                if (disc[nb] == -1) {
                    tj(adj, nb, low, disc, onStack, st, res, time);
                    low[u] = std::min(low[u], low[nb]);                
                }
                else if (onStack[nb]) low[u] = std::min(low[u], disc[nb]);
            }

            if (disc[u] == low[u]) {
                std::vector<int> tmp;
                int w {};

                do {
                    w = st.top();
                    st.pop();
                    onStack[w] = false;
                    tmp.push_back(w);
                } while (w != u);

                res.push_back(tmp);
            }
        }
    //Kosaraju
        std::vector<std::vector<int>> kj(std::vector<std::vector<int>>& adj) {
            std::vector<std::vector<int>> radj(adj.size());
            for (int i{}; i < adj.size(); ++i) {
                for (int nb : adj[i]) radj[nb].push_back(i);
            }

            std::vector<std::vector<int>> res;
            std::vector<bool> visited(adj.size());
            std::stack<int> st;

            for (int i{}; i < adj.size(); ++i) {
                if (!visited[i]) dfs1(i, adj, visited, st);
            }
 
            std::fill(visited.begin(), visited.end(), false);

            while (!st.empty()) {
                int curr = st.top();
                st.pop();
                std::vector<int> tmp;

                if (!visited[curr]) { 
                    dfs2(curr, radj, visited, tmp);
                    res.push_back(tmp);
                }
            }
            return res;
        }
    
        void dfs1(int u, std::vector<std::vector<int>>& adj, std::vector<bool>& visited, std::stack<int>& st) {
            visited[u] = true;
            
            for (int nb : adj[u]) {
                if (!visited[nb]) dfs1(nb, adj, visited, st);
            }

            st.push(u);
        }

        void dfs2(int u, std::vector<std::vector<int>>& radj, std::vector<bool>& visited, std::vector<int>& tmp) {
            visited[u] = true;
            tmp.push_back(u);
            
            for (int nb : radj[u]) {
                if (!visited[nb]) dfs2(nb, radj, visited, tmp);        
            }
        }


    //Shortest path on DAGs bfs
        std::vector<int> dagBfs(int n, int startNode, std::vector<std::vector<std::pair<int, int>>>& adj) {
            std::vector<int> indeg(n);
            for (int i{}; i < n; ++i) {
                for (auto [nb, w] : adj[i]) {
                    indeg[nb]++;
                }
            } 
        
            std::queue<int> q;
            for (int i{}; i < n; ++i) {
                if (!indeg[i]) q.push(i);
            } 

            std::vector<int> dist(n, std::numeric_limits<int>::max());
            dist[startNode] = 0;

            while (!q.empty()) {
                int curr = q.front();
                q.pop();

                if (dist[curr] != std::numeric_limits<int>::max()) {
                    for (auto [nb, w] : adj[curr]) {
                        if (dist[curr] + w < dist[nb]) {
                            dist[nb] = dist[curr] + w;
                        }
                    }
                }

                for (auto [nb, w] : adj[curr]) {
                    if (--indeg[nb] == 0) q.push(nb);
                }
            }
            return dist;
        }

    //Shortest path on DAGs dfs
        std::vector<int> dagDfs (int n, int src, std::vector<std::vector<std::pair<int, int>>> adj) {
            std::vector<int> visited(n);
            std::vector<int> dist(n, std::numeric_limits<int>::max());
            std::stack<int> st;
            dist[src] = 0;

            for (int i{}; i < n; ++i) {
                if (!visited[i]) dagDfs(i, adj, visited, st);
            }

            while (!st.empty()) {
                int curr = st.top(); st.pop();

                if (dist[curr] == std::numeric_limits<int>::max()) continue;
                for (auto [nb, w]: adj[curr]) {
                    if (dist[curr] + w < dist[nb]) dist[nb] = dist[curr] + w;
                }
            }
            return dist;
        }
        
        void dagDfs(int u, std::vector<std::vector<std::pair<int, int>>>& adj, std::vector<int>& visited, std::stack<int>& st) {
            visited[u] = true;

            for (auto [nb, w] : adj[u]) {
                if (!visited[nb]) dagDfs(nb, adj, visited, st);
            }
            st.push(u);
        }

    //Bellman-Ford edge list
        std::vector<int> dist;
        bool BF(int src, int v, std::vector<std::vector<int>> edges) {
            dist.assign(v, std::numeric_limits<int>::max());
            dist[src] = 0;

            for (int i{}; i < v - 1; ++i) {
                bool updated = false;
                for (int j{}; j < edges.size(); ++j) {
                    int u = edges[j][0];
                    int nb = edges[j][1];
                    int w = edges[j][2];

                    if (dist[u] != std::numeric_limits<int>::max()) {
                        if (dist[u] + w < dist[nb]) {
                            dist[nb] = dist[u] + w;
                            updated = true;
                        }
                    }
                }
                if (!updated) return true;
            }
            
            for (int j{}; j < edges.size(); ++j) {
                int u = edges[j][0];
                int nb = edges[j][1];
                int w = edges[j][2];

                if (dist[u] != std::numeric_limits<int>::max()) {
                    if (dist[u] + w < dist[nb]) return false;
                }
            }
            return true;
        }

     //Bellman-Ford adjacency list
        std::vector<int> dist;
        bool BF(int src, int v, std::vector<std::vector<std::tuple<int, int, int>>> adj) {     
            dist.assign(v, std::numeric_limits<int>::max());
            dist[src] = 0;

            for (int i{}; i < v - 1; ++i) {
                bool updated = false;

                for (int j{}; j < v; ++j) {
                    for (auto [u, nb, w] : adj[j]) { 
                        if (dist[u] != std::numeric_limits<int>::max()) {
                            if (dist[u] + w < dist[nb]) {
                                dist[nb] = dist[u] + w;
                                updated = true;
                            }
                        }
                    }
                }

                if (!updated) return true;
            }
            
           for (int j{}; j < v; ++j) {
                for (auto [u, nb, w] : adj[j]) {
                    if (dist[u] != std::numeric_limits<int>::max()) {
                        if (dist[u] + w < dist[nb]) return false;
                    }
                }
            }
            return true;
        }
};

#endif