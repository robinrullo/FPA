//
// Created by Robin RULLO on 23/01/2024.
//

#ifndef RANDN_HPP
#define RANDN_HPP

#include <random>

inline std::default_random_engine generator;
inline std::normal_distribution<double> normal_distribution(0, 1);

inline double randn()
{
    return(normal_distribution(generator));
}



#endif //RANDN_HPP
