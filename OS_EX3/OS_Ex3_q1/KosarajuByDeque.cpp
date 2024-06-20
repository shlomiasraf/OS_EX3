#include <iostream>
#include <vector>
#include <deque>

void DFS1(int v, std::vector<std::vector<int>>& adjMatrix, std::vector<bool>& visited, std::deque<int>& Deque, int n)
{
    visited[v] = true;
    for (int i = 0; i < n; ++i)
    {
        if (adjMatrix[v][i] && !visited[i])  // Check if there's an edge from v to i
        {
            DFS1(i, adjMatrix, visited, Deque, n);
        }
    }
    Deque.push_back(v);  // use deque to store the finish time order
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

void kosaraju(int n, std::vector<std::vector<int>>& adjMatrix)
{
    std::deque<int> Deque;
    std::vector<bool> visited(n, false);

    // Step 1: Perform DFS on the original graph to fill the finish time stack (Deque)
    for (int i = 0; i < n; ++i)
    {
        if (!visited[i])
        {
            DFS1(i, adjMatrix, visited, Deque, n);
        }
    }

    // Step 2: Transpose the adjacency matrix
    std::vector<std::vector<int>> transposed_adjMatrix(n, std::vector<int>(n, 0));
    for (int v = 0; v < n; ++v)
    {
        for (int i = 0; i < n; ++i)
        {
            transposed_adjMatrix[i][v] = adjMatrix[v][i];
        }
    }

    // Step 3: Clear visited array and process vertices in order defined by finish time (Deque)
    std::fill(visited.begin(), visited.end(), false);
    while (!Deque.empty())
    {
        int v = Deque.back();
        Deque.pop_back();
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

    // Initialize adjacency matrix with all zeros
    std::vector<std::vector<int>> adjMatrix(vertex, std::vector<int>(vertex, 0));

    std::cout << "Please enter the edges: " << std::endl;
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        std::cin >> u >> v;
        adjMatrix[u - 1][v - 1] = 1;  // Assuming edges are 1-based and setting the adjacency matrix
    }

    kosaraju(vertex, adjMatrix);
    return 0;
}

