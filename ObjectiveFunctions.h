//
// Created by Robin RULLO on 22/01/2024.
//

#ifndef OBJECTIVE_FUNCTIONS_HPP
#define OBJECTIVE_FUNCTIONS_HPP

#include <cmath>

class ObjectiveFunction
{
public:
    virtual ~ObjectiveFunction() = default;

    virtual double compute_fitness(const double& x) const
    {
        return 0.;
    };

    virtual std::string getName() const = 0;
};

class Rosenbrock final : public ObjectiveFunction
{
public:
    double compute_fitness(const double& x) const override
    {
        double sum = 0;
        sum += std::pow(1 - x, 2) + 100 * std::pow(x, 2);
        return sum;
    }

    std::string getName() const override
    {
        return name;
    }

private:
    const char* name = "Rosenbrock";
};

class Rastrigin final : public ObjectiveFunction
{
public:
    double compute_fitness(const double& x) const override
    {
        double sum = 0;
        sum += std::pow(x, 2) - 10 * std::cos(2 * M_PI * x) + 10;
        return sum;
    }

    std::string getName() const override
    {
        return name;
    }

private:
    const char* name = "Rastrigin";
};

class Ackley final : public ObjectiveFunction
{
public:
    double compute_fitness(const double& x) const override
    {
        double sum = 0;
        sum += -20 * std::exp(-0.2 * std::sqrt(0.5 * std::pow(x, 2))) - std::exp(0.5 * std::cos(2 * M_PI * x)) + 20 +
            std::exp(1);
        return sum;
    }

    std::string getName() const override
    {
        return name;
    }

private:
    const char* name = "Ackley";
};

class Sphere final : public ObjectiveFunction
{
public:
    std::string getName() const override
    {
        return name;
    }

    double compute_fitness(const double& x) const override
    {
        return std::pow(x, 2);
    }

private:
    const char* name = "Sphere";
};

#endif //OBJECTIVE_FUNCTIONS_HPP
