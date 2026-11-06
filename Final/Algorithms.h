#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Employee.h"

// ---------- BINARY SEARCH (array must be sorted by id) ----------
// Best O(1), Average O(log n), Worst O(log n). Returns index or -1.
inline int binarySearchById(const Employee* arr, int n, int id) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid].id == id) return mid;
        if (arr[mid].id < id) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ---------- LINEAR SEARCH ----------
// Best O(1), Average O(n), Worst O(n).

// On an array. Returns index or -1.
template <typename T, typename Pred>
int linearSearchArray(const T* arr, int n, Pred match) {
    for (int i = 0; i < n; i++)
        if (match(arr[i])) return i;
    return -1;
}

// On a linked list (nodes have a "next" pointer). Starts at "head" and
// returns the first matching node, or nullptr.
template <typename Node, typename Pred>
Node* linearSearchList(Node* head, Pred match) {
    for (Node* c = head; c != nullptr; c = c->next)
        if (match(*c)) return c;
    return nullptr;
}
/ ---------- MERGE SORT ----------
// Best, Average, Worst: O(n log n). Extra space O(n). Stable.
// "less(a, b)" must return true when a should come before b.
template <typename T, typename Less>
void mergeRange(T* a, T* tmp, int lo, int mid, int hi, Less less) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (!less(a[j], a[i])) tmp[k++] = a[i++];  // keeps equal items in order
        else tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi) tmp[k++] = a[j++];
    for (k = lo; k <= hi; k++) a[k] = tmp[k];
}

template <typename T, typename Less>
void mergeSortRange(T* a, T* tmp, int lo, int hi, Less less) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortRange(a, tmp, lo, mid, less);
    mergeSortRange(a, tmp, mid + 1, hi, less);
    mergeRange(a, tmp, lo, mid, hi, less);
}

template <typename T, typename Less>
void mergeSort(T* a, int n, Less less) {
    if (n < 2) return;
    T* tmp = new T[n];
    mergeSortRange(a, tmp, 0, n - 1, less);
    delete[] tmp;
}


#endif