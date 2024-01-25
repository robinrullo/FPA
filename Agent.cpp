//
// Created by Robin RULLO on 23/01/2024.
//

#include "Agent.h"
#include <iostream>

Agent::Agent(const std::vector<double>& solution, const double fitness) : solution(solution), fitness(fitness), id(increase())
{
}

int Agent::increase()
{
    return ++ID;
}

Agent Agent::copy() const
{
    Agent agent_copy(solution, fitness);
    return agent_copy;
}

void Agent::print() const
{
    std::cout << "id: " << id << ", target: " << std::setprecision(17) << fitness << ", solution: [";
    for (const auto& val : solution)
    {
        std::cout << val << ", ";
    }
    std::cout << "]" << std::endl;
}
