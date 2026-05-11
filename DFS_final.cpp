#include<iostream>      // For input and output
#include<omp.h>         // For OpenMP parallel programming
#include<chrono>        // For measuring execution time
#include<vector>        // For adjacency list representation
#include<stack>         // For DFS traversal using stack

using namespace std;
using namespace chrono;

// Graph class
class Graph{

    public:

        int vertices;                     // Number of vertices
        vector<vector<int>> graph;       // Adjacency list
        vector<bool> visited;            // Visited array

        // Constructor
        Graph(int v){

            this->vertices = v;

            // Resize adjacency list
            this->graph.resize(v);

            // Initialize visited array with false
            this->visited.resize(v, false);
        }

        // Reset visited array before traversal
        void reset_visited(){

            for (int i = 0; i < this->vertices; i++){

                this->visited[i] = false;
            }
        }

        // Add edge between two vertices
        void add_edge(int a, int b){

            // Add edge a -> b
            this->graph[a].push_back(b);

            // Add edge b -> a (undirected graph)
            this->graph[b].push_back(a);
        }

        // Sequential DFS function
        void sequential_dfs(int start){

            // Reset visited array
            reset_visited();

            // Create stack for DFS
            stack<int> s;

            // Push starting vertex
            s.push(start);

            // Mark as visited
            visited[start] = true;

            // Continue until stack becomes empty
            while(!s.empty()){

                // Get top element
                int current = s.top();

                // Remove top element
                s.pop();

                // Print current node
                cout << current << " ";

                // Traverse all neighbors
                for (int i = 0; i < this->graph[current].size(); i++){

                    int neighbour = this->graph[current][i];

                    // If neighbor is not visited
                    if (!this->visited[neighbour]){

                        // Push neighbor into stack
                        s.push(neighbour);

                        // Mark as visited
                        this->visited[neighbour] = true;
                    }
                }
            }

            cout << endl;
        }

        // Parallel DFS function
        void parallel_dfs(int start){

            // Reset visited array
            reset_visited();

            // Start OpenMP parallel region
            #pragma omp parallel
            {
                // Only one thread executes dfs_task
                #pragma omp single
                {
                    dfs_task(start);
                }
            }
        }

        // DFS helper function for parallel execution
        void dfs_task(int start){

            // Stack for DFS traversal
            stack<int> s;

            // Push starting vertex
            s.push(start);

            // Mark starting vertex visited
            visited[start] = true;

            // Continue until stack is empty
            while(!s.empty()){

                // Get top vertex
                int current = s.top();

                // Remove top vertex
                s.pop();

                // Print current vertex
                cout << current << " ";

                // Parallel loop for neighbors
                #pragma omp parallel for
                for (int i = 0; i < this->graph[current].size(); i++){

                    int neighbour = this->graph[current][i];

                    // If neighbor not visited
                    if (!this->visited[neighbour]){

                        // Critical section to avoid race condition
                        #pragma omp critical
                        {
                            // Double-check visited status
                            if (!this->visited[neighbour]){

                                // Push neighbor into stack
                                s.push(neighbour);

                                // Mark visited
                                this->visited[neighbour] = true;
                            }
                        }
                    }
                }
            }

            cout << endl;
        }
};

// Main function
int main(){

    int V, E;

    // Input number of vertices
    cout << "Enter number of vertices: ";
    cin >> V;

    // Input number of edges
    cout << "Enter number of edges: ";
    cin >> E;

    // Create graph object
    Graph g(V);

    // Input edges
    cout << "Enter edges:\n";

    for (int i = 0; i < E; i++){

        int a, b;

        cin >> a >> b;

        // Add edge to graph
        g.add_edge(a, b);
    }

    int startVertex;

    // Input starting vertex
    cout << "Enter starting vertex: ";
    cin >> startVertex;

    // Start timer for sequential DFS
    auto start = high_resolution_clock::now();

    cout << "\nSequential DFS: ";

    // Call sequential DFS
    g.sequential_dfs(startVertex);

    // End timer
    auto end = high_resolution_clock::now();

    // Calculate execution time
    double time1 = duration<double>(end-start).count();

    // Print sequential DFS time
    cout << "Sequential DFS Time: " << time1 << endl;

    // Start timer for parallel DFS
    start = high_resolution_clock::now();

    cout << "\nParallel DFS: ";

    // Call parallel DFS
    g.parallel_dfs(startVertex);

    // End timer
    end = high_resolution_clock::now();

    // Calculate parallel execution time
    double time2 = duration<double>(end-start).count();

    // Print parallel DFS time
    cout << "Parallel DFS Time: " << time2 << endl;

    // Print speedup
    cout << "SpeedUp in DFS: " << time1 / time2 << endl;

    return 0;
}

/*Enter number of vertices: 10
Enter number of edges: 14
Enter edges:
0 1                                         
0 2
1 3
1 4
2 5
2 6
3 7
4 7
5 8
6 8
7 9
8 9
3 5
4 6
Enter starting vertex: 0

Sequential DFS: 0 2 6 4 7 9 3 8 5 1 
Sequential DFS Time: 0.05641

Parallel DFS: 0 2 6 4 7 9 3 8 5 1 
Parallel DFS Time: 0.004329
SpeedUp in DFS: 13.0307*/