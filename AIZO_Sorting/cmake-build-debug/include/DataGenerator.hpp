#ifndef DATA_GENERATOR_HPP
#define DATA_GENERATOR_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <type_traits>

enum class RangeOption {
    Limited,
    Full
};

template <typename T>
std::vector<T> loadArrayFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<T> arr;
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return arr;
    }
    int size;
    file >> size;
    if (size <= 0) {
        std::cerr << "Nieprawidłowy rozmiar tablicy: " << size << std::endl;
        return arr;
    }
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
        T value;
        file >> value;
        arr[i] = value;
    }
    return arr;
}

template <typename T>
std::vector<T> generateRandomArray(int size, RangeOption rangeOpt) {
    std::vector<T> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());

    if (rangeOpt == RangeOption::Limited) {
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<int> dist(0, 100);
            for (int i = 0; i < size; i++) {
                arr[i] = static_cast<T>(dist(gen));
            }
        } else {
            std::uniform_real_distribution<float> dist(0.0f, 100.0f);
            for (int i = 0; i < size; i++) {
                arr[i] = static_cast<T>(dist(gen));
            }
        }
    } else {
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<int> dist(-1000000, 1000000);
            for (int i = 0; i < size; i++) {
                arr[i] = static_cast<T>(dist(gen));
            }
        } else {
            std::uniform_real_distribution<double> dist(-1e6, 1e6);
            for (int i = 0; i < size; i++) {
                arr[i] = static_cast<T>(dist(gen));
            }
        }
    }
    return arr;
}

template <typename T>
void printArray(const std::vector<T>& arr) {
    for (auto& val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

template <typename T>
bool isSorted(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

#endif // DATA_GENERATOR_HPP
