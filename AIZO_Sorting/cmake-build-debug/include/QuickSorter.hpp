#ifndef QUICK_SORTER_HPP
#define QUICK_SORTER_HPP

#include "ISorter.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>

enum class PivotStrategy {
    Left,
    Right,
    Middle,
    Random
};

template <typename T>
class QuickSorter : public ISorter<T> {
public:
    QuickSorter(PivotStrategy strategy = PivotStrategy::Left)
        : strategy_(strategy) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    void sort(std::vector<T>& data) override {
        if (!data.empty()) {
            quickSortRecursive(data, 0, data.size() - 1);
        }
    }

private:
    PivotStrategy strategy_;

    void quickSortRecursive(std::vector<T>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortRecursive(arr, low, pi - 1);
            quickSortRecursive(arr, pi + 1, high);
        }
    }

    int partition(std::vector<T>& arr, int low, int high) {
        int pivotIndex = choosePivot(low, high);
        std::swap(arr[pivotIndex], arr[high]);
        T pivot = arr[high];

        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    int choosePivot(int low, int high) {
        switch (strategy_) {
            case PivotStrategy::Left:
                return low;
            case PivotStrategy::Right:
                return high;
            case PivotStrategy::Middle:
                return low + (high - low) / 2;
            case PivotStrategy::Random:
                return low + (std::rand() % (high - low + 1));
        }
        return high; // domyślnie
    }
};

#endif // QUICK_SORTER_HPP
