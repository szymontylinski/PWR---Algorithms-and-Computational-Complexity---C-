#include <iostream>
#include "App.hpp"

int main() {
    // Wybór typu danych
    int typeChoice;
    std::cout << "Wybierz typ danych (1 - int, 2 - float): ";
    std::cin >> typeChoice;

    if (typeChoice == 2) {
        App<float> app;
        app.run();
    } else {
        App<int> app;
        app.run();
    }
    return 0;
}
