#include <iostream>     // For input and output
#include <vector>       // For vector container
#include <chrono>       // For execution time calculation
#include <omp.h>        // For OpenMP parallel programming

using namespace std;
using namespace chrono;

// Sequential Bubble Sort Function
void bubbleSort(vector<int> &arr)
{
    // Get size of array
    int n = arr.size();

    // Outer loop for passes
    for(int i = 0; i < n-1; i++)
    {
        // Inner loop for comparison
        for(int j = 0; j < n-i-1; j++)
        {
            // Swap if elements are in wrong order
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort using Odd-Even Sort
void oddEvenSort(vector<int> &arr)
{
    // Get size of array
    int n = arr.size();

    // Flag to check sorting completion
    bool sorted = false;

    // Repeat until array becomes sorted
    while(!sorted)
    {
        sorted = true;

        // ---------------- Odd Phase ----------------

        // Parallel comparison of odd indexed elements
        #pragma omp parallel for
        for(int i = 1; i < n-1; i += 2)
        {
            // Swap if needed
            if(arr[i] > arr[i+1])
            {
                swap(arr[i], arr[i+1]);

                sorted = false;
            }
        }

        // ---------------- Even Phase ----------------

        // Parallel comparison of even indexed elements
        #pragma omp parallel for
        for(int i = 0; i < n-1; i += 2)
        {
            // Swap if needed
            if(arr[i] > arr[i+1])
            {
                swap(arr[i], arr[i+1]);

                sorted = false;
            }
        }
    }
}

// Function to print array
void printVector(vector<int> &arr)
{
    // Traverse vector
    for(int x : arr)
    {
        cout << x << " ";
    }

    cout << endl;
}

// Main Function
int main()
{
    int n;

    // Input number of elements
    cout << "Enter number of elements: ";
    cin >> n;

    // Vector declaration
    vector<int> arr(n);

    // Input elements
    cout << "Enter elements:\n";

    for(int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    // Temporary vector for sorting
    vector<int> temp;

    // ---------------- Sequential Bubble Sort ----------------

    temp = arr;

    // Start timer
    auto start = high_resolution_clock::now();

    // Call sequential bubble sort
    bubbleSort(temp);

    // End timer
    auto end = high_resolution_clock::now();

    // Calculate execution time
    double time1 = duration<double>(end - start).count();

    // Display sorted array
    cout << "\nSequential Bubble Sort: ";
    printVector(temp);

    // Display execution time
    cout << "Time: " << time1 << endl << endl;

    // ---------------- Parallel Bubble Sort ----------------

    temp = arr;

    // Start timer
    start = high_resolution_clock::now();

    // Call parallel bubble sort
    oddEvenSort(temp);

    // End timer
    end = high_resolution_clock::now();

    // Calculate execution time
    double time2 = duration<double>(end - start).count();

    // Display sorted array
    cout << "Parallel Bubble Sort: ";
    printVector(temp);

    // Display execution time
    cout << "Time: " << time2 << endl;

    // Display speedup
    cout << "SpeedUp: " << time1 / time2 << endl;

    return 0;
}