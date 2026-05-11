#include<iostream>      // For input and output
#include<omp.h>         // For OpenMP parallel programming
#include<chrono>        // For measuring execution time
#include<vector>        // For adjacency list representation
#include<queue>         // For BFS traversal using queue

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

        // Sequential BFS function
        void sequential_bfs(int start){

            // Reset visited array
            reset_visited();

            // Create queue for BFS
            queue<int> s;

            // Push starting vertex
            s.push(start);

            // Mark starting vertex as visited
            visited[start] = true;

            // Continue until queue becomes empty
            while(!s.empty()){

                // Get front element
                int current = s.front();

                // Remove front element
                s.pop();

                // Print current node
                cout << current << " ";

                // Traverse all neighbors
                for (int i = 0; i < this->graph[current].size(); i++){

                    int neighbour = this->graph[current][i];

                    // If neighbor is not visited
                    if (!this->visited[neighbour]){

                        // Push neighbor into queue
                        s.push(neighbour);

                        // Mark as visited
                        this->visited[neighbour] = true;
                    }
                }
            }

            cout << endl;
        }

        // Parallel BFS function
        void parallel_bfs(int start){

            // Reset visited array
            reset_visited();

            // Start OpenMP parallel region
            #pragma omp parallel
            {
                // Only one thread executes bfs_task
                #pragma omp single
                {
                    bfs_task(start);
                }
            }
        }

        // BFS helper function for parallel execution
        void bfs_task(int start){

            // Queue for BFS traversal
            queue<int> s;

            // Push starting vertex
            s.push(start);

            // Mark starting vertex visited
            visited[start] = true;

            // Continue until queue is empty
            while(!s.empty()){

                // Get front vertex
                int current = s.front();

                // Remove front vertex
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

                                // Push neighbor into queue
                                s.push(neighbour);

                                // Mark neighbor as visited
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

    // Input graph edges
    cout << "Enter edges:\n";

    for (int i = 0; i < E; i++){

        int a, b;

        cin >> a >> b;

        // Add edge into graph
        g.add_edge(a, b);
    }

    int startVertex;

    // Input starting vertex
    cout << "Enter starting vertex: ";
    cin >> startVertex;

    // Start timer for sequential BFS
    auto start = high_resolution_clock::now();

    cout << "\nSequential BFS: ";

    // Call sequential BFS
    g.sequential_bfs(startVertex);

    // End timer
    auto end = high_resolution_clock::now();

    // Calculate sequential BFS time
    double time1 = duration<double>(end-start).count();

    // Print sequential BFS time
    cout << "Sequential BFS Time: " << time1 << endl;

    // Start timer for parallel BFS
    start = high_resolution_clock::now();

    cout << "\nParallel BFS: ";

    // Call parallel BFS
    g.parallel_bfs(startVertex);

    // End timer
    end = high_resolution_clock::now();

    // Calculate parallel BFS time
    double time2 = duration<double>(end-start).count();

    // Print parallel BFS time
    cout << "Parallel BFS Time: " << time2 << endl;

    // Print speedup
    cout << "SpeedUp in BFS: " << time1 / time2 << endl;

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

Sequential BFS: 0 1 2 3 4 5 6 7 8 9 
Sequential BFS Time: 0.01614

Parallel BFS: 0 1 2 3 4 5 6 7 8 9 
Parallel BFS Time: 0.00555
SpeedUp in BFS: 2.90811*/