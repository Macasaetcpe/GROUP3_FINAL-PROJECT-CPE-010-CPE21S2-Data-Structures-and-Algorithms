// Benchmark for the performance analysis (Section 5.2).
// Measures: binary search, linear search, merge sort, queue enqueue.
// Each measurement is repeated RUNS times and the average is printed.

#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <vector>

#include "Algorithms.h"
#include "Employee.h"
#include "Queue.h"

using Clock = std::chrono::steady_clock;

const int RUNS = 5;
const int LOOKUPS = 1000;  // number of searches done per measurement

static double elapsedMs(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

int main() {
    const int sizes[] = {100, 1000, 10000, 100000};
    volatile long long sink = 0;  // stops the compiler from removing the work

    std::printf("Setup: %d runs per size (average), %d lookups per search test, fixed seed\n\n",
                RUNS, LOOKUPS);
    std::printf("%-10s %-22s %-22s %-18s %-18s\n", "n", "BinarySearch x1000(ms)",
                "LinearSearch x1000(ms)", "MergeSort(ms)", "Enqueue n(ms)");

    for (int n : sizes) {
        std::mt19937 rng(12345);  // same data every time

        // Employees with sorted unique IDs (needed by binary search)
        std::vector<Employee> emp(n);
        for (int i = 0; i < n; i++) {
            emp[i].id = i * 3 + 1;
            emp[i].name = "Emp" + std::to_string(i);
        }
        // Random IDs to look for (all exist)
        std::vector<int> targets(LOOKUPS);
        for (int i = 0; i < LOOKUPS; i++) targets[i] = emp[rng() % n].id;

        // Unsorted copy for merge sort
        std::vector<Employee> unsorted = emp;
        for (int i = n - 1; i > 0; i--) std::swap(unsorted[i], unsorted[rng() % (i + 1)]);

        double bs = 0, ls = 0, ms = 0, eq = 0;
        for (int r = 0; r < RUNS; r++) {
            auto t0 = Clock::now();
            for (int i = 0; i < LOOKUPS; i++) sink += binarySearchById(emp.data(), n, targets[i]);
            auto t1 = Clock::now();
            bs += elapsedMs(t0, t1);

            t0 = Clock::now();
            for (int i = 0; i < LOOKUPS; i++) {
                int id = targets[i];
                sink += linearSearchArray(emp.data(), n, [id](const Employee& e) { return e.id == id; });
            }
            t1 = Clock::now();
            ls += elapsedMs(t0, t1);

            std::vector<Employee> copy = unsorted;  // copy is outside the timing
            t0 = Clock::now();
            mergeSort(copy.data(), n, [](const Employee& a, const Employee& b) { return a.id < b.id; });
            t1 = Clock::now();
            ms += elapsedMs(t0, t1);
            sink += copy[0].id;

            Queue<int> q;
            t0 = Clock::now();
            for (int i = 0; i < n; i++) q.enqueue(i);
            t1 = Clock::now();
            eq += elapsedMs(t0, t1);
            sink += q.size();
        }
        std::printf("%-10d %-22.4f %-22.4f %-18.4f %-18.4f\n", n, bs / RUNS, ls / RUNS, ms / RUNS,
                    eq / RUNS);
    }
    return 0;
}
