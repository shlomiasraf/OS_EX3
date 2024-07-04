#include <iostream>
#include <vector>
#include <list>

void DFS1(int v, std::vector<std::vector<int>>& adjMatrix, std::vector<bool>& visited, std::list<int>& List, int n)
{
    visited[v] = true;
    for (int i = 0; i < n; ++i)
    {
        if (adjMatrix[v][i] && !visited[i])  // Check if there's an edge from v to i
        {
            DFS1(i, adjMatrix, visited, List, n);
        }
    }
    List.push_front(v);  // use list (deque) to store the finish time order
}

void DFS2(int v, std::vector<std::vector<int>>& adjMatrix, std::vector<bool>& visited, std::vector<int>& component, int n)
{
    visited[v] = true;
    component.push_back(v);
    for (int i = 0; i < n; ++i)
    {
        if (adjMatrix[v][i] && !visited[i])  // Check if there's an edge from v to i
        {
            DFS2(i, adjMatrix, visited, component, n);
        }
    }
}

void kosaraju(int n, std::vector<std::list<int>>& adj)
{
    std::list<int> List;
    std::vector<bool> visited(n, false);

    // Step 1: Convert adjacency list to adjacency matrix
    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n, 0));
    for (int v = 0; v < n; ++v)
    {
        for (int i : adj[v])
        {
            adjMatrix[v][i] = 1;  // Assuming edges are unweighted and setting the adjacency matrix
        }
    }

    // Step 2: Perform DFS on the original graph to fill the finish time stack (List)
    for (int i = 0; i < n; ++i)
    {
        if (!visited[i])
        {
            DFS1(i, adjMatrix, visited, List, n);
        }
    }

    // Step 3: Transpose the adjacency matrix
    std::vector<std::vector<int>> transposed_adjMatrix(n, std::vector<int>(n, 0));
    for (int v = 0; v < n; ++v)
    {
        for (int i = 0; i < n; ++i)
        {
            transposed_adjMatrix[i][v] = adjMatrix[v][i];
        }
    }

    // Step 4: Clear visited array and process vertices in order defined by finish time (List)
    std::fill(visited.begin(), visited.end(), false);
    for (int v : List)
    {
        if (!visited[v])
        {
            std::vector<int> component;
            DFS2(v, transposed_adjMatrix, visited, component, n);
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

    // Initialize adjacency list with empty lists
    std::vector<std::list<int>> adj(vertex);

    std::cout << "Please enter the edges: " << std::endl;
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        std::cin >> u >> v;
        adj[u - 1].push_back(v - 1);  // Assuming edges are 1-based and setting the adjacency list
    }

    kosaraju(vertex, adj);
    return 0;
}

