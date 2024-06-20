#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

enum class Command{
    Newgraph,
    Kosaraju,
    Newedge,
    Removeedge,
    Exit,
    Invalid
};

Command getCommandFromString(const std::string& commandStr) {
    std::string lowerCommand = commandStr;
    std::transform(lowerCommand.begin(), lowerCommand.end(), lowerCommand.begin(), ::tolower);

    if (lowerCommand == "newgraph") {
        return Command::Newgraph;
    } else if (lowerCommand == "kosaraju") {
        return Command::Kosaraju;
    } else if (lowerCommand == "newedge") {
        return Command::Newedge;
    } else if (lowerCommand == "removeedge") {
        return Command::Removeedge;
    } else if (lowerCommand == "exit") {
        return Command::Exit;
    }
    else {
        return Command::Invalid;
    }
}

void DFS1(int v, std::vector<std::list<int>>& adj, std::vector<bool>& visited, std::list<int>& List)
{
    visited[v] = true;
    for (int i : adj[v])
    {
        if (!visited[i])
        {
            DFS1(i, adj, visited, List);
        }
    }
    List.push_front(v);  // use list (deque) to store the finish time order
}

void DFS2(int v, std::vector<std::list<int>>& adj, std::vector<bool>& visited, std::vector<int>& component)
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

void kosaraju(std::vector<std::list<int>>& adj)
{
    int n=adj.size();
    std::list<int> List;
    std::vector<bool> visited(n, false);

    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            DFS1(i, adj, visited, List);
        }
    }

    std::vector<std::list<int>> transposed_adj(n);
    for (int v = 0; v < n; v++)
    {
        for (int i : adj[v])
        {
            transposed_adj[i].push_front(v);
        }
    }

    fill(visited.begin(), visited.end(), false);
    for (int v : List)
    {
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

std::vector<std::list<int>> Newgraph(std::vector<std::list<int>> &adj, int vertex,int edges){
    std::cout << "Please enter the number of vertices and edges: ";
    std::cin >> vertex >> edges;
    std::cout << "Please enter the edges: " << std::endl;
    adj.resize(vertex);
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        std::cin >> u >> v;
        adj[u - 1].push_back(v - 1);
    }
    std::cout<<std::endl;
    return adj;
}

void Newedge(std::vector<std::list<int>> &adj){

    int i,j;
    std::cout<<"Please enter edge you wish to add\n";
    std::cin>>i,j;
    adj[i].push_back(j);
}

void Removeedge(std::vector<std::list<int>> &adj){
    int i, j;
    std::cout << "Enter edge to remove (i j): ";
    std::cin >> i >> j;

    // Check if vertex i exists in the adjacency list and if j is in its list
    if (i >= 0 && i < adj.size() && j >= 0 && j < adj.size()) {
        bool found = false;

        // Traverse the list at index i to find and remove vertex j
        for (auto it = adj[i].begin(); it != adj[i].end(); ++it) {
            if (*it == j) {
                adj[i].erase(it);
                found = true;
                break;
            }
        }

        if (found) {
            std::cout << "Edge (" << i  << ", " << j  << ") removed.\n" << std::endl;
        } else {
            std::cout << "Edge (" << i  << ", " << j  << ") not found.\n" << std::endl;
        }
    } else {
        std::cout << "Invalid vertices!\n" << std::endl;
    }
}


int main()
{
    int vertex=0, edges=0;
    std::vector<std::list<int>> adj(vertex);
    std::string input;

    Command command = Command::Invalid;
    while (command != Command::Exit) {

        std::cout << "Enter command (Newgraph, Kosaraju, Newedge, Removeedge): \n";
        std::cin>>input;
        command=getCommandFromString(input);

        switch (command) {
            case Command::Newgraph:
                Newgraph(adj,vertex,edges);
                break;

            case Command::Kosaraju:
                kosaraju(adj);
                break;

            case Command::Newedge:
                Newedge(adj);
                break;

            case Command::Removeedge:
                Removeedge(adj);
                break;

            case Command::Invalid:
                std::cout << "Invalid command!" << std::endl;
                break;

            case Command::Exit:
                break;
        }
    }
    return 0;
}
