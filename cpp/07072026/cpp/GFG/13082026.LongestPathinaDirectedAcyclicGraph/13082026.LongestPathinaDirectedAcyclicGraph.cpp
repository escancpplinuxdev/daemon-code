
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

class Solution 
{
  public:
    vector<int> maxDistance(int V, int src, vector<vector<int>>& edges) 
{
        // Build adjacency list and indegree for topological sort
        vector<vector<pair<int, int>>> adj(V);
        vector<int> indegree(V, 0);

        for (auto &e : edges) {
            int u = e[0], v = e[1], w = e[2];
            adj[u].push_back({v, w});
            indegree[v]++;
        }

        // Kahn's algorithm for topological order
        queue<int> q;
        for (int i = 0; i < V; i++) {
            if (indegree[i] == 0) q.push(i);
        }

        vector<int> topo;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            topo.push_back(u);
            for (auto &p : adj[u]) {
                int v = p.first;
                if (--indegree[v] == 0) q.push(v);
            }
        }

        // Longest distances
        const int NEG_INF = INT_MIN;
        vector<int> dist(V, NEG_INF);
        dist[src] = 0;

        for (int u : topo) {
            if (dist[u] == NEG_INF) continue;
            for (auto &p : adj[u]) {
                int v = p.first, w = p.second;
                if (dist[v] < dist[u] + w) {
                    dist[v] = dist[u] + w;
                }
            }
        }

        return dist;
    }
};


int main()
{
// Input: V = 4, src = 0, edges[][] = [[0, 1, 1], [0, 2, 1], [1, 2, 5], [3, 1, 2], [3, 2, -1]]
// Output: [0, 1, 6, INF]

	Solution sol;
	int V = 4, src = 0;
	std::vector<vector<int>> edges = {{0, 1, 1}, {0, 2, 1}, {1, 2, 5}, {3, 1, 2}, {3, 2, -1}};
	
	std::vector<int> v = sol.maxDistance(V,src,edges);

        for (auto x : v)
        {
                std::cout<<x<<" ";
        }

                std::cout<<"\n";

	return 0;
}

/*
Longest Path in a Directed Acyclic Graph
Difficulty: HardAccuracy: 50.38%Submissions: 10K+Points: 8

Given a weighted Directed Acyclic Graph (DAG) with V vertices numbered from 0 to V - 1, represented by edges[][], where edges[i] = [u, v, w] denotes a directed edge from u to v with weight w, and a source vertex src.

    Return the distance array, where the value at index i represents the longest distance from s to vertex i.
    If a vertex is unreachable from s, store INT_MIN for that vertex. The driver code will automatically display INT_MIN as INF.

Examples :

Input: V = 4, src = 0, edges[][] = [[0, 1, 1], [0, 2, 1], [1, 2, 5], [3, 1, 2], [3, 2, -1]]
Output: [0, 1, 6, INF]
Explanation: The longest distance of vertex 1 from 0 is 1, vertex 2 is 6 and vertex 3 is unreachable so INF.

Input: V = 5, src = 1, edges[][] = [[0, 1, 1], [0, 2, 2], [1, 4, 4], [3, 2, -1], [4, 2, 3], [4, 3, 6]]
Output: [INF, 0, 9, 10, 4]
Explanation: The vertex 0 is not reachable from vertex 1 so its distance is INF, for 2 it is 9, for 3 it is 10, and for 4 it is 4.

class Solution {
  public:
    vector<int> maxDistance(int V, int src, vector<vector<int>> &edges) {
        // code here
        
    }
};

*/
