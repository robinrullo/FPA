//
// Created by Robin RULLO on 23/01/2024.
//

#ifndef LEVY_HPP
#define LEVY_HPP

#include "randn.hpp"
#include <vector>
#include <math.h>

class Levy
{
public:
    static std::vector<double> getLevyFlightStep(const double& beta, const double& multiplier, const int& size, const int& caseValue)
    {
        if (size < 2)
        {
            throw std::invalid_argument("size must be greater than 1. Size = 1 is not implemented.");
        }

        std::vector<double> u(size), v(size);
        for (int i = 0; i < size; ++i)
        {
            const double sigma_u = std::pow(
                tgamma(1. + beta) * std::sin(M_PI * beta / 2) / (tgamma((1 + beta) / 2.) * beta * std::pow(
                    2., (beta - 1) / 2)), 1. / beta);
            constexpr double sigma_v = 1;
            u[i] = std::normal_distribution<double>(0, std::pow(sigma_u, 2))(generator);
            v[i] = std::normal_distribution<double>(0, std::pow(sigma_v, 2))(generator);
        }

        std::vector<double> s(size);
        for (int i = 0; i < size; ++i)
        {
            s[i] = u[i] / std::pow(std::abs(v[i]), 1 / beta);
        }

        std::vector<double> step;
        if (caseValue == 0)
        {
            for (int i = 0; i < size; ++i)
            {
                step.push_back(multiplier * s[i] * std::uniform_real_distribution<double>(0, 1)(generator));
            }
        }
        else if (caseValue == 1)
        {
            for (int i = 0; i < size; ++i)
            {
                step.push_back(multiplier * s[i] * std::normal_distribution<double>(0, 1)(generator));
            }
        }
        else
        {
            for (int i = 0; i < size; ++i)
            {
                step.push_back(multiplier * s[i]);
            }
        }
        return step;
    }
};

#endif //LEVY_HPP
