#include <iostream>
#include <vector>
#include <deque>

void DFS1(int v, std::vector<std::deque<int>>& adj, std::vector<bool>& visited, std::deque<int>& Deque)
{
    visited[v] = true;
    for (int i : adj[v])
    {
        if (!visited[i])
        {
            DFS1(i, adj, visited, Deque);
        }
    }
    Deque.push_back(v);  // use deque to store the finish time order
}

void DFS2(int v, std::vector<std::deque<int>>& adj, std::vector<bool>& visited, std::vector<int>& component)
{
    visited[v] = true;
    component.push_back(v);
    for (int i : adj[v])
    {
        if (!visited[i])
        {
            DFS2(i, adj, visited, component);
        }
    }
}

void kosaraju(int n, std::vector<std::deque<int>>& adj)
{
    std::deque<int> Deque;
    std::vector<bool> visited(n, false);

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            DFS1(i, adj, visited, Deque);
        }
    }

    std::vector<std::deque<int>> transposed_adj(n);
    for (int v = 0; v < n; v++)
    {
        for (int i : adj[v])
        {
            transposed_adj[i].push_back(v);
        }
    }

    fill(visited.begin(), visited.end(), false);
    while (!Deque.empty())
    {
        int v = Deque.back();
        Deque.pop_back();
        if (!visited[v])
        {
            std::vector<int> component;
            DFS2(v, transposed_adj, visited, component);
            std::cout << "SCC: ";
            for (int i : component)
            {
                std::cout << i + 1 << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main()
{
    int vertex, edges;
    std::cout << "Please enter number of vertices and edges: ";
    std::cin >> vertex >> edges;
    std::vector<std::deque<int>> adj(vertex);

    std::cout << "Please enter the edges: " << std::endl;
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        std::cin >> u >> v;
        adj[u - 1].push_back(v - 1);
    }

    kosaraju(vertex, adj);
    return 0;
}
