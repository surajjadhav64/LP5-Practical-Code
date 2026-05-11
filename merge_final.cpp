#include <iostream>     // For input and output
#include <vector>       // For vector container
#include <chrono>       // For execution time calculation
#include <omp.h>        // For OpenMP parallel programming

using namespace std;
using namespace chrono;

// Merge Function
void merge(vector<int> &arr, int left, int mid, int right)
{
    // Temporary vector
    vector<int> temp;

    // Starting index of left subarray
    int i = left;

    // Starting index of right subarray
    int j = mid + 1;

    // Merge two sorted subarrays
    while(i <= mid && j <= right)
    {
        if(arr[i] < arr[j])
            temp.push_back(arr[i++]);

        else
            temp.push_back(arr[j++]);
    }

    // Copy remaining left elements
    while(i <= mid)
        temp.push_back(arr[i++]);

    // Copy remaining right elements
    while(j <= right)
        temp.push_back(arr[j++]);

    // Copy sorted data back into original array
    for(int k = 0; k < temp.size(); k++)
    {
        arr[left + k] = temp[k];
    }
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int> &arr, int left, int right)
{
    // Continue if more than one element exists
    if(left < right)
    {
        // Find middle index
        int mid = (left + right) / 2;

        // Sort left half
        sequentialMergeSort(arr, left, mid);

        // Sort right half
        sequentialMergeSort(arr, mid + 1, right);

        // Merge sorted halves
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int> &arr, int left, int right)
{
    // Continue if more than one element exists
    if(left < right)
    {
        // Find middle index
        int mid = (left + right) / 2;

        // Parallel sections for left and right halves
        #pragma omp parallel sections
        {
            // Left half sorting
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            // Right half sorting
            #pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }

        // Merge sorted halves
        merge(arr, left, mid, right);
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

    // ---------------- Sequential Merge Sort ----------------

    temp = arr;

    // Start timer
    auto start = high_resolution_clock::now();

    // Call sequential merge sort
    sequentialMergeSort(temp, 0, temp.size()-1);

    // End timer
    auto end = high_resolution_clock::now();

    // Calculate execution time
    double time1 = duration<double>(end - start).count();

    // Display sorted array
    cout << "\nSequential Merge Sort: ";
    printVector(temp);

    // Display execution time
    cout << "Time: " << time1 << endl << endl;

    // ---------------- Parallel Merge Sort ----------------

    temp = arr;

    // Start timer
    start = high_resolution_clock::now();

    // Call parallel merge sort
    parallelMergeSort(temp, 0, temp.size()-1);

    // End timer
    end = high_resolution_clock::now();

    // Calculate execution time
    double time2 = duration<double>(end - start).count();

    // Display sorted array
    cout << "Parallel Merge Sort: ";
    printVector(temp);

    // Display execution time
    cout << "Time: " << time2 << endl;

    // Display speedup
    cout << "SpeedUp: " << time1 / time2 << endl;

    return 0;
}