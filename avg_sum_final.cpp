#include<iostream>      // For input and output
#include<omp.h>         // For OpenMP parallel programming
#include<iomanip>       // For formatting output
#include<chrono>        // For measuring execution time

using namespace std;

// Function to calculate serial sum
int sum_serial(int arr[], int n) 
{
    int sum = 0;

    // Traverse array sequentially
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }

    return sum;
}

// Function to calculate parallel sum
int sum_parallel(int arr[], int n) 
{
    int sum = 0;

    // Parallel reduction for sum
    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }

    return sum;
}

// Function to calculate serial average
int average_serial(int arr[], int n) 
{
    int sum = 0;

    // Traverse array sequentially
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }

    // Return average
    return sum / n;
}

// Function to calculate parallel average
int average_parallel(int arr[], int n) 
{
    int sum = 0;

    // Parallel reduction for sum
    #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; ++i)
    {
        sum += arr[i];
    }

    // Return average
    return sum / n;
}

// Main function
int main() 
{
    int n;

    // Input number of elements
    cout << "Enter number of elements: ";
    cin >> n;

    // Array declaration
    int arr[n];

    // Input array elements
    cout << "Enter elements:\n";

    for (int i = 0; i < n; ++i)
    {
        cin >> arr[i];
    }

    // ---------------- SUM ----------------

    // Start timer for serial sum
    auto ss_start = chrono::high_resolution_clock::now();

    // Call serial sum function
    int sum_s = sum_serial(arr, n);

    // End timer
    auto ss_end = chrono::high_resolution_clock::now();

    // Start timer for parallel sum
    auto sp_start = chrono::high_resolution_clock::now();

    // Call parallel sum function
    int sum_p = sum_parallel(arr, n);

    // End timer
    auto sp_end = chrono::high_resolution_clock::now();

    // Calculate execution times
    chrono::duration<double> d_ss = ss_end - ss_start;
    chrono::duration<double> d_sp = sp_end - sp_start;

    // Display results
    cout << "\nSUM\n\n";

    cout << "Serial result: " << sum_s << endl;

    cout << "Parallel result: " << sum_p << endl;

    cout << "Serial duration: " 
         << fixed << setprecision(10)
         << d_ss.count() << " seconds\n";

    cout << "Parallel duration: " 
         << d_sp.count() << " seconds\n";

    // Calculate speedup
    cout << "Speedup: " 
         << d_ss.count() / d_sp.count() 
         << "\n\n";

    // ---------------- AVERAGE ----------------

    // Start timer for serial average
    auto as_start = chrono::high_resolution_clock::now();

    // Call serial average function
    int avg_s = average_serial(arr, n);

    // End timer
    auto as_end = chrono::high_resolution_clock::now();

    // Start timer for parallel average
    auto ap_start = chrono::high_resolution_clock::now();

    // Call parallel average function
    int avg_p = average_parallel(arr, n);

    // End timer
    auto ap_end = chrono::high_resolution_clock::now();

    // Calculate execution times
    chrono::duration<double> d_as = as_end - as_start;
    chrono::duration<double> d_ap = ap_end - ap_start;

    // Display results
    cout << "AVERAGE\n\n";

    cout << "Serial result: " << avg_s << endl;

    cout << "Parallel result: " << avg_p << endl;

    cout << "Serial duration: " 
         << d_as.count() << " seconds\n";

    cout << "Parallel duration: " 
         << d_ap.count() << " seconds\n";

    // Calculate speedup
    cout << "Speedup: " 
         << d_as.count() / d_ap.count() 
         << "\n\n";

    return 0;
}