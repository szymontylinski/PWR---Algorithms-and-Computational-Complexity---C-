#ifndef SHELL_SORTER_HPP
#define SHELL_SORTER_HPP

#include "ISorter.hpp"
#include <vector>

// Dwie strategie odstępów
enum class ShellStrategy {
    Classic,      // gap = n/2, potem gap/2 itd.
    FrankLazarus  // podobnie, ale w osobnej funkcji generującej odstępy
};

template <typename T>
class ShellSorter : public ISorter<T> {
public:
    ShellSorter(ShellStrategy strategy = ShellStrategy::Classic)
        : strategy_(strategy) {}

    void sort(std::vector<T>& data) override {
        shellSort(data);
    }

private:
    ShellStrategy strategy_;

    void shellSort(std::vector<T>& arr) {
        int n = arr.size();
        if (n < 2) return;

        if (strategy_ == ShellStrategy::Classic) {
            // Klasyczny gap
            for (int gap = n / 2; gap > 0; gap /= 2) {
                for (int i = gap; i < n; i++) {
                    T temp = arr[i];
                    int j = i;
                    while (j >= gap && arr[j - gap] > temp) {
                        arr[j] = arr[j - gap];
                        j -= gap;
                    }
                    arr[j] = temp;
                }
            }
        } else {
            // Frank i Lazarus - przykładowe podejście
            auto gaps = generateFrankLazarusGaps(n);
            for (int gap : gaps) {
                for (int i = gap; i < n; i++) {
                    T temp = arr[i];
                    int j = i;
                    while (j >= gap && arr[j - gap] > temp) {
                        arr[j] = arr[j - gap];
                        j -= gap;
                    }
                    arr[j] = temp;
                }
            }
        }
    }

    std::vector<int> generateFrankLazarusGaps(int n) {
        // Przykład podobny do klasycznego, można tu rozbudować logikę
        std::vector<int> gaps;
        int gap = n / 2;
        while (gap > 0) {
            gaps.push_back(gap);
            gap /= 2;
        }
        return gaps;
    }
};

#endif // SHELL_SORTER_HPP
