#ifndef INSERTION_SORTER_HPP
#define INSERTION_SORTER_HPP

#include "ISorter.hpp"
#include <vector>

template <typename T>
class InsertionSorter : public ISorter<T> {
public:
    void sort(std::vector<T>& data) override {
        int n = data.size();
        for (int i = 1; i < n; i++) {
            T key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }
};

#endif // INSERTION_SORTER_HPP
