#ifndef MY_HEAP_SORTER_HPP
#define MY_HEAP_SORTER_HPP

#include "ISorter.hpp"
#include <algorithm>
#include <vector>

template <typename T>
class MyHeapSorter : public ISorter<T> {
public:
    void sort(std::vector<T>& data) override {
        heapSort(data);
    }

private:
    void heapify(std::vector<T>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }
        if (largest != i) {
            std::swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }

    void heapSort(std::vector<T>& arr) {
        int n = arr.size();
        // Budowanie kopca
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }
        // "Ściąganie" elementów z kopca
        for (int i = n - 1; i >= 0; i--) {
            std::swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }
};

#endif // MY_HEAP_SORTER_HPP
