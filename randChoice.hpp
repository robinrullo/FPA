//
// Created by Robin RULLO on 24/01/2024.
//

#ifndef RANDCHOICE_HPP
#define RANDCHOICE_HPP
#include <vector>

#include "randn.hpp"


template <typename T>
int randomChoice(const std::vector<T>& array) {
    if (array.empty()) {
        return 0;
    }

    // Create a uniform distribution over the range [0, array.size())
    std::uniform_int_distribution<> distribution(0, array.size() - 1);

    // Generate a random index
    return distribution(generator);
}

#endif //RANDCHOICE_HPP
