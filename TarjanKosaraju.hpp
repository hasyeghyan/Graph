#ifndef TARJANKOSARAJU_HPP
#define TARJANKOSARAJU_HPP

#include <vector>
#include <stack>
#include <algorithm>

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
};

#endif