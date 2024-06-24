#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#define PORT "9034"

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
    std::cout << "Please enter the number of vertices and edges: \n";
    std::cin >> vertex >> edges;
    std::cout << "Please enter the edges: " << std::endl;
    std::cout << vertex;
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
    std::cin>>i;
    std::cin>>j;
    adj[i].push_back(j);
}

void Removeedge(std::vector<std::list<int>> &adj){
    int i, j;
    std::cout << "Enter edge to remove (i j): ";
    std::cin >> i >> j;

    // Check if vertex i exists in the adjacency list and if j is in its list
    if (i >= 0 && (long unsigned int)i < adj.size() && j >= 0 && (long unsigned int)j < adj.size()) {
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

void Command_Shift(std::string input){

    int vertex=0, edges=0;
    std::vector<std::list<int>> adj(vertex);
    
    Command command = Command::Invalid;
    if (command != Command::Exit) {
        
        command = getCommandFromString(input);

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
}

int setup_server() {

    int server_fd;
    struct sockaddr_in  address;

                            
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9034);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", 9034);
    return server_fd;
}
// Function to add a client.
void setup_poll_connection(int server_fd, std::vector<pollfd> &poll_fds) {
    int sock;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    
    // Create socket
    if((sock = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0){ // Accepts a new client connection and creates a new socket for that connection
        perror("accept");
    }else{
    std::cout << "New connection, socket fd is " << sock << ", ip is: "
              << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;

    // Add new client to poll_fds
    pollfd client_pollfd = {sock, POLLIN, 0};
    poll_fds.push_back(client_pollfd);
    }
}

void del_from_pfds(std::vector<pollfd> &poll_fds, int i, int *fd_count)
{
    // Copy the one from the end over this one
    poll_fds[i] = poll_fds[*fd_count-1];

    (*fd_count)--;
}

void handle_recieve_data(std::vector<pollfd> &poll_fds,int index, int *fd_count){
// If not the listener, we're just a regular client
    char buf[256];
    int nbytes = recv(poll_fds[index].fd, buf, sizeof buf, 0);

    int sender_fd = poll_fds[index].fd;

    while (nbytes <= 0) {
     // Got error or connection closed by client
        if (nbytes == 0) {
                            // Connection closed
          printf("pollserver: socket %d hung up\n", sender_fd);
                 } else {
                        perror("recv");
                        }
                        close(poll_fds[index].fd); // Bye!
    }   
   
        std::string input(buf);
        if (!input.empty()) {
        input.pop_back();
        }
         pid_t pid = fork();
        if (pid == 0) {
        // Child process
        std::vector<std::list<int>> adj;
        Command_Shift(input);
        close(sender_fd);
        exit(0);
    } else if (pid < 0) {
        perror("fork failed");
    }
        //del_from_pfds(poll_fds,index,fd_count);
    
}


int main()
{
    // Set up server
    int server_fd = setup_server();
    if (server_fd < 0) {
        std::cerr << "Failed to set up server\n";
        return 1;
    }


    std::vector<struct pollfd> poll_fds;
    pollfd server_pollfd = {server_fd, POLLIN, 0};
    poll_fds.push_back(server_pollfd);
    int fd_count = 1;

    while (true) {
        // Use poll to monitor file descriptors
        int activity = poll(poll_fds.data(), poll_fds.size(), -1);
        if (activity < 0) {
            perror("poll error");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        for (int i=0; (size_t)i<poll_fds.size(); i++) {
            if (poll_fds.at(i).revents & POLLIN) {
                if (poll_fds.at(i).fd == server_fd) {
                    // Accept new connection
                    setup_poll_connection(server_fd,poll_fds);
                } else {
                    // Handle incoming message from a client
                    
                    handle_recieve_data(poll_fds,i,&fd_count);
                  
                    }
             }
        }
    }
    return 0;
}
