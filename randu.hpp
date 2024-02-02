//
// Created by Robin RULLO on 23/01/2024.
//

#ifndef RANDU_HPP
#define RANDU_HPP

#include <random>

inline std::uniform_real_distribution<double> uniform_distribution(0.0,1.0);
inline double randu()
{
    return(uniform_distribution(generator));
}


#endif //RANDU_HPP
