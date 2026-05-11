#include<iostream>      // For input and output
#include<omp.h>         // For OpenMP parallel programming
#include<iomanip>       // For formatting output
#include<chrono>        // For measuring execution time

using namespace std;

// Function to find minimum value serially
int min_serial(int arr[], int n) 
{
    // Assume first element as minimum
    int min_val = arr[0];

    // Traverse remaining array
    for (int i = 1; i < n; ++i)
    {
        // Update minimum value
        if (arr[i] < min_val)
        {
            min_val = arr[i];
        }
    }

    return min_val;
}

// Function to find minimum value using parallel reduction
int min_parallel(int arr[], int n) 
{
    // Assume first element as minimum
    int min_val = arr[0];

    // Parallel reduction for minimum value
    #pragma omp parallel for reduction(min : min_val)
    for (int i = 1; i < n; ++i)
    {
        // Compare elements
        if (arr[i] < min_val)
        {
            min_val = arr[i];
        }
    }

    return min_val;
}

// Function to find maximum value serially
int max_serial(int arr[], int n) 
{
    // Assume first element as maximum
    int max_val = arr[0];

    // Traverse remaining array
    for (int i = 1; i < n; ++i)
    {
        // Update maximum value
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }

    return max_val;
}

// Function to find maximum value using parallel reduction
int max_parallel(int arr[], int n) 
{
    // Assume first element as maximum
    int max_val = arr[0];

    // Parallel reduction for maximum value
    #pragma omp parallel for reduction(max : max_val)
    for (int i = 1; i < n; ++i)
    {
        // Compare elements
        if (arr[i] > max_val)
        {
            max_val = arr[i];
        }
    }

    return max_val;
}

// Main function
int main() 
{
    int n;

    // Input number of elements
    cout << "Enter number of elements: ";
    cin >> n;

    // Declare array
    int arr[n];

    // Input array elements
    cout << "Enter elements:\n";

    for (int i = 0; i < n; ++i)
    {
        cin >> arr[i];
    }

    // ---------------- MINIMUM ----------------

    // Start timer for serial minimum
    auto mins_start = chrono::high_resolution_clock::now();

    // Call serial minimum function
    int min_s = min_serial(arr, n);

    // End timer
    auto mins_end = chrono::high_resolution_clock::now();

    // Start timer for parallel minimum
    auto minp_start = chrono::high_resolution_clock::now();

    // Call parallel minimum function
    int min_p = min_parallel(arr, n);

    // End timer
    auto minp_end = chrono::high_resolution_clock::now();

    // Calculate execution times
    chrono::duration<double> d_mins = mins_end - mins_start;
    chrono::duration<double> d_minp = minp_end - minp_start;

    // Display minimum results
    cout << "\nMIN\n\n";

    cout << "Serial result: " << min_s << endl;

    cout << "Parallel result: " << min_p << endl;

    cout << "Serial duration: "
         << d_mins.count() << " seconds\n";

    cout << "Parallel duration: "
         << d_minp.count() << " seconds\n";

    // Calculate speedup
    cout << "Speedup: "
         << d_mins.count() / d_minp.count()
         << "\n\n";

    // ---------------- MAXIMUM ----------------

    // Start timer for serial maximum
    auto maxs_start = chrono::high_resolution_clock::now();

    // Call serial maximum function
    int max_s = max_serial(arr, n);

    // End timer
    auto maxs_end = chrono::high_resolution_clock::now();

    // Start timer for parallel maximum
    auto maxp_start = chrono::high_resolution_clock::now();

    // Call parallel maximum function
    int max_p = max_parallel(arr, n);

    // End timer
    auto maxp_end = chrono::high_resolution_clock::now();

    // Calculate execution times
    chrono::duration<double> d_maxs = maxs_end - maxs_start;
    chrono::duration<double> d_maxp = maxp_end - maxp_start;

    // Display maximum results
    cout << "MAX\n\n";

    cout << "Serial result: " << max_s << endl;

    cout << "Parallel result: " << max_p << endl;

    cout << "Serial duration: "
         << d_maxs.count() << " seconds\n";

    cout << "Parallel duration: "
         << d_maxp.count() << " seconds\n";

    // Calculate speedup
    cout << "Speedup: "
         << d_maxs.count() / d_maxp.count()
         << "\n\n";

    return 0;
}