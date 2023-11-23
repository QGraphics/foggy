//
// Created by omar on 11/24/23.
//

#include <iostream>
#include <ctime>

int main() {
    std::time_t result = std::time(nullptr);
    std::cout << std::asctime(std::localtime(&result)) << std::endl;
    return 0;
}
