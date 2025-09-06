#ifndef APP_HPP
#define APP_HPP

#include <vector>
#include <chrono>
#include <iostream>
#include <memory>
#include <functional>
#include "DataGenerator.hpp"
#include "ISorter.hpp"

// Klasy sortujące:
#include "InsertionSorter.hpp"
#include "MyHeapSorter.hpp"
#include "ShellSorter.hpp"
#include "QuickSorter.hpp"

template <typename T>
class App {
public:
    App() : rangeOption(RangeOption::Limited) {}

    void run();

private:
    RangeOption rangeOption;
    std::vector<T> currentArray;

    void displayMainMenu();
    void handleMenuChoice(int choice);

    void loadArray();
    void generateRandom();
    void printCurrent();
    void sortCurrent();

    // Metoda zautomatyzowanych testów
    void runBatchTests();

    // ZAMIANA: zamiast unique_ptr<ISorter<T>>& przekazujemy ISorter<T>*.
    long long measureSortTime(ISorter<T>* sorter, const std::vector<T>& data, int iterations);
};

template <typename T>
void App<T>::run() {
    bool running = true;
    while (running) {
        displayMainMenu();
        int choice;
        std::cin >> choice;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (choice == 5) {
            running = false;
        } else {
            handleMenuChoice(choice);
        }
    }
}

template <typename T>
void App<T>::displayMainMenu() {
    std::cout << "\n----------------------------------------\n";
    std::cout << "        MENU GLOWNE - SORTOWANIE        \n";
    std::cout << "----------------------------------------\n";
    std::cout << "1. Wczytaj tablice z pliku\n";
    std::cout << "2. Wygeneruj losowa tablice\n";
    std::cout << "3. Wyswietl aktualna tablice\n";
    std::cout << "4. Sortuj tablice\n";
    std::cout << "5. Wyjdz z programu\n";
    std::cout << "6. Testy automatyczne\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Wybor: ";
}

template <typename T>
void App<T>::handleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        loadArray();
        break;
    case 2:
        generateRandom();
        break;
    case 3:
        printCurrent();
        break;
    case 4:
        sortCurrent();
        break;
    case 6:
        runBatchTests();
        break;
    default:
        std::cout << "Nieprawidlowy wybor, sprobuj ponownie.\n";
        break;
    }
}

template <typename T>
void App<T>::loadArray() {
    std::string filename;
    std::cout << "Podaj nazwe pliku: ";
    std::cin >> filename;
    currentArray = loadArrayFromFile<T>(filename);
    if (!currentArray.empty()) {
        std::cout << "Tablica wczytana.\n";
    }
}

template <typename T>
void App<T>::generateRandom() {
    int size;
    std::cout << "Podaj rozmiar tablicy: ";
    std::cin >> size;
    if (size <= 0) {
        std::cout << "Rozmiar nieprawidlowy.\n";
        return;
    }
    int rangeChoice;
    std::cout << "Wybierz zakres (1 - ograniczony, 2 - pelny): ";
    std::cin >> rangeChoice;
    rangeOption = (rangeChoice == 2 ? RangeOption::Full : RangeOption::Limited);

    currentArray = generateRandomArray<T>(size, rangeOption);
    std::cout << "Losowa tablica utworzona.\n";
}

template <typename T>
void App<T>::printCurrent() {
    if (currentArray.empty()) {
        std::cout << "Brak tablicy.\n";
        return;
    }
    std::cout << "Aktualna tablica:\n";
    printArray(currentArray);
}

template <typename T>
void App<T>::sortCurrent() {
    if (currentArray.empty()) {
        std::cout << "Brak tablicy do sortowania.\n";
        return;
    }
    std::cout << "\nWybierz algorytm sortowania:\n";
    std::cout << "1. Insertion Sort\n";
    std::cout << "2. Heap Sort\n";
    std::cout << "3. Shell Sort\n";
    std::cout << "4. Quick Sort\n";
    std::cout << "Wybor: ";
    int algChoice;
    std::cin >> algChoice;

    std::unique_ptr<ISorter<T>> sorter;

    switch (algChoice) {
    case 1:
        sorter = std::make_unique<InsertionSorter<T>>();
        break;
    case 2:
        sorter = std::make_unique<MyHeapSorter<T>>();
        break;
    case 3: {
        int sChoice;
        std::cout << "1 - Klasyczna sekwencja, 2 - Frank & Lazarus: ";
        std::cin >> sChoice;
        ShellStrategy str = (sChoice == 2 ? ShellStrategy::FrankLazarus : ShellStrategy::Classic);
        sorter = std::make_unique<ShellSorter<T>>(str);
        break;
    }
    case 4: {
        int pivotChoice;
        std::cout << "Pivot: 1 - Lewy, 2 - Prawy, 3 - Srodkowy, 4 - Losowy: ";
        std::cin >> pivotChoice;
        PivotStrategy ps;
        switch (pivotChoice) {
            case 1: ps = PivotStrategy::Left;   break;
            case 2: ps = PivotStrategy::Right;  break;
            case 3: ps = PivotStrategy::Middle; break;
            case 4: ps = PivotStrategy::Random; break;
            default: ps = PivotStrategy::Left;  break;
        }
        sorter = std::make_unique<QuickSorter<T>>(ps);
        break;
    }
    default:
        std::cout << "Nieprawidlowy wybor.\n";
        return;
    }

    // Mierzymy czas
    std::vector<T> arrayCopy = currentArray;
    auto start = std::chrono::high_resolution_clock::now();
    sorter->sort(arrayCopy);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (!isSorted(arrayCopy)) {
        std::cout << "Blad: tablica nie jest posortowana!\n";
    }
    std::cout << "Czas sortowania: " << duration << " ms\n";
    std::cout << "Tablica po sortowaniu:\n";
    printArray(arrayCopy);
}

// Pomocnicza funkcja do wielokrotnych pomiarów
// ZAMIAST: measureSortTime(std::unique_ptr<ISorter<T>>& sorter, ...)
// MAMY:    measureSortTime(ISorter<T>* sorter, ...)
template <typename T>
long long App<T>::measureSortTime(ISorter<T>* sorter,
                                  const std::vector<T>& data,
                                  int iterations)
{
    long long totalTime = 0;
    for (int i = 0; i < iterations; i++) {
        std::vector<T> copyArr = data;
        auto start = std::chrono::high_resolution_clock::now();
        sorter->sort(copyArr);
        auto end = std::chrono::high_resolution_clock::now();
        if (!isSorted(copyArr)) {
            std::cout << "Blad: tablica nie jest posortowana!\n";
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        totalTime += duration;
    }
    return totalTime / iterations;
}

// Zautomatyzowane testy batchowe
template <typename T>
void App<T>::runBatchTests() {
    // Rozmiary tablic
    std::vector<int> sizes = {10000, 20000, 30000, 40000, 50000, 60000, 70000};
    // Liczba powtórzeń
    int iterations = 100;

    auto genSorted = [&](int size) {
        auto arr = generateRandomArray<T>(size, rangeOption);
        // posortujemy rosnąco insertion sortem
        InsertionSorter<T> localIns;
        localIns.sort(arr);
        return arr;
    };
    auto genReverse = [&](int size) {
        auto arr = genSorted(size);
        std::reverse(arr.begin(), arr.end());
        return arr;
    };
    auto genPartial33 = [&](int size) {
        auto arr = generateRandomArray<T>(size, rangeOption);
        int index33 = size * 33 / 100;
        std::vector<T> temp(arr.begin(), arr.begin() + index33);
        InsertionSorter<T> localIns;
        localIns.sort(temp);
        for (int i = 0; i < index33; ++i) {
            arr[i] = temp[i];
        }
        return arr;
    };
    auto genPartial66 = [&](int size) {
        auto arr = generateRandomArray<T>(size, rangeOption);
        int index66 = size * 66 / 100;
        std::vector<T> temp(arr.begin(), arr.begin() + index66);
        InsertionSorter<T> localIns;
        localIns.sort(temp);
        for (int i = 0; i < index66; ++i) {
            arr[i] = temp[i];
        }
        return arr;
    };
    auto genRandom = [&](int size) {
        return generateRandomArray<T>(size, rangeOption);
    };

    std::vector<std::pair<std::string, std::function<std::vector<T>(int)>>> variants = {
        {"Losowa",               genRandom},
        {"Posortowana rosnaco",  genSorted},
        {"Posortowana malejaco", genReverse},
        {"Czesciowo (33%)",      genPartial33},
        {"Czesciowo (66%)",      genPartial66}
    };

    std::cout << "\n=== Testy automatyczne ("
              << (std::is_same<T,int>::value ? "int" : "float")
              << ") ===\n";

    for (int sz : sizes) {
        std::cout << "\nRozmiar tablicy: " << sz << "\n";

        for (auto& variant : variants) {
            std::cout << " --- " << variant.first << " ---\n";
            std::vector<T> baseArr = variant.second(sz);

            // 1) Insertion
            {
                auto sorter = std::make_unique<InsertionSorter<T>>();
                long long timeMs = measureSortTime(sorter.get(), baseArr, iterations);
                std::cout << "Insertion Sort: " << timeMs << " ms\n";
            }
            // 2) Heap
            {
                auto sorter = std::make_unique<MyHeapSorter<T>>();
                long long timeMs = measureSortTime(sorter.get(), baseArr, iterations);
                std::cout << "Heap Sort: " << timeMs << " ms\n";
            }
            // 3) Shell (2 strategie)
            {
                auto sorterClassic = std::make_unique<ShellSorter<T>>(ShellStrategy::Classic);
                long long timeMs = measureSortTime(sorterClassic.get(), baseArr, iterations);
                std::cout << "Shell Sort (Classic): " << timeMs << " ms\n";

                auto sorterFrank = std::make_unique<ShellSorter<T>>(ShellStrategy::FrankLazarus);
                timeMs = measureSortTime(sorterFrank.get(), baseArr, iterations);
                std::cout << "Shell Sort (Frank & Lazarus): " << timeMs << " ms\n";
            }
            // 4) Quick Sort (4 pivoty)
            for (auto pivot : {PivotStrategy::Left, PivotStrategy::Right,
                               PivotStrategy::Middle, PivotStrategy::Random})
            {
                auto sorter = std::make_unique<QuickSorter<T>>(pivot);
                long long timeMs = measureSortTime(sorter.get(), baseArr, iterations);

                std::string pivotName;
                switch (pivot) {
                    case PivotStrategy::Left:   pivotName = "Left";   break;
                    case PivotStrategy::Right:  pivotName = "Right";  break;
                    case PivotStrategy::Middle: pivotName = "Middle"; break;
                    case PivotStrategy::Random: pivotName = "Random"; break;
                }
                std::cout << "Quick Sort (pivot=" << pivotName << "): " << timeMs << " ms\n";
            }
        }
    }
}

#endif // APP_HPP
