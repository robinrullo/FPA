//
// Created by Robin RULLO on 23/01/2024.
//

#include "FPA.h"
#include <ranges>

#include "Agent.h"
#include "Levy.hpp"
#include "randChoice.hpp"
#include "randu.hpp"

int nfe_counter = 0;

FPA::FPA(
    const int& epoch,
    const int& pop_size,
    const double& pc,
    const double& pm,
    const double& p_s,
    const double& levy_multiplier,
    const ObjectiveFunction* objectiveFunc,
    const std::pair<std::vector<double>, std::vector<double>>& bounds,
    const std::string& minmax,
    const int& dimensions
)
{
    this->epoch = epoch;
    this->pop_size = pop_size;
    this->pc = pc;
    this->pm = pm;
    this->p_s = p_s;
    this->levy_multiplier = levy_multiplier;
    this->objectiveFunc = objectiveFunc;
    this->bounds = bounds;
    this->minmax = minmax;
    this->dimensions = dimensions;

    this->g_best = Agent();
}


Agent& FPA::solve()
{
    // Initialize population
    this->pop = this->generatePopulation();
    // Update global best solution
    this->g_best = updateGlobalBestAgent(this->pop).second;

    for (int epoch = 1; epoch < this->epoch + 1; ++epoch)
    {
        this->evolve(epoch);
    }
    // Update global best solution, the population is sorted or not depended on algorithm's strategy
    this->g_best = updateGlobalBestAgent(this->pop).second;
    return this->g_best;
}

void FPA::evolve(const double& epoch)
{
    auto pop = std::vector<Agent>(this->pop_size);
    for (int idx = 0; idx < this->pop_size; ++idx)
    {
        auto pos_new = Solution(this->dimensions);

        if (randu() < this->p_s)
        {
            auto levy = Levy::getLevyFlightStep(1.0, this->levy_multiplier, this->dimensions, -1);
            for (int i = 0; i < this->dimensions; ++i)
            {
                const auto pos = this->pop[idx].solution[i] + 1.0 / std::sqrt(epoch) * levy[i] * (this->pop[idx].
                    solution[i]
                    - this->g_best.solution[i]);
                pos_new[i] = pos;
            }
        }
        else
        {
            // Get only population from 0 to idx
            auto selected_pop = std::vector<Agent>(this->pop.begin(), this->pop.end() - idx);
            const auto id1 = randomChoice(selected_pop);
            const auto id2 = randomChoice(selected_pop);
            for (int i = 0; i < this->dimensions; ++i)
            {
                pos_new[i] = this->pop[idx].solution[i] + randu() * (this->pop[id1].solution[i] - this->pop[id2].
                    solution[i]);
            }
        }
        pos_new = this->correctSolution(pos_new, this->bounds);
        auto agent = Agent(pos_new);
        pop.push_back(agent);

        agent.fitness = this->getFitness(pos_new);
        const auto agentsToCompare = std::make_pair(this->pop[idx], agent);
        this->pop[idx] = FPA::getBetterAgent(agentsToCompare, this->minmax, false);
    }
};

std::pair<std::vector<std::pair<Agent, int>>, Agent> FPA::updateGlobalBestAgent(const std::vector<Agent>& pop)
{
    // Update global best and current best solutions in history object.
    // Also update global worst and current worst solutions in history object.

    std::vector<std::pair<Agent, int>> sorted_pop = FPA::getSortedPopulation(pop, this->minmax);
    Agent c_best = sorted_pop.front().first;

    // If the global best is not initialized, we return it for initialization.
    if (this->g_best.solution.empty())
    {
        return std::make_pair(sorted_pop, c_best);
    }
    // Check if the current best is better than the global best
    Agent better = FPA::getBetterAgent(std::make_pair(c_best, this->g_best), this->minmax, false);

    return std::make_pair(sorted_pop, better);
}

std::vector<std::pair<Agent, int>> FPA::getSortedPopulation(const std::vector<Agent>& pop,
                                                            const std::string& minmax = "min")
{
    // initialize a vector of fitnesses
    auto fitnesses = std::vector<double>();
    for (const auto& i : pop)
    {
        fitnesses.push_back(i.fitness);
    }

    if (minmax == "max")
    {
        std::ranges::sort(fitnesses, std::greater{});
    }
    else
    {
        std::ranges::sort(fitnesses, std::less{});
    }

    std::vector<int> indices(fitnesses.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::ranges::sort(indices, [&fitnesses](const int i, const int j) { return fitnesses[i] < fitnesses[j]; });

    auto new_pop = std::vector<std::pair<Agent, int>>();
    for (const int idx : indices)
    {
        new_pop.emplace_back(pop[idx], idx);
    }

    return new_pop;
}

Agent FPA::getBetterAgent(const std::pair<Agent, Agent>& agents, const std::string& minmax = "min",
                          const bool& reverse = false)
{
    // Get the better agent based on fitness.

    std::unordered_map<std::string, int> minmax_dict{{"min", 0}, {"max", 1}};
    int idx = minmax_dict[minmax];

    if (reverse)
    {
        idx = 1 - idx;
    }

    if (idx == 0)
    {
        return (agents.first.fitness < agents.second.fitness)
                   ? agents.first.copy()
                   : agents.second.copy();
    }
    return (agents.first.fitness > agents.second.fitness)
               ? agents.first.copy()
               : agents.second.copy();
}


std::vector<Agent> FPA::generatePopulation() const
{
    // Generate a population of agents.
    auto pop = std::vector<Agent>(this->pop_size);
    for (auto& agent : pop)
    {
        Solution solution = this->generateSolution();
        agent = Agent(solution);
        agent.fitness = this->getFitness(agent.solution);
    }

    return pop;
}

Solution FPA::generateSolution() const
{
    const auto lb = this->bounds.first;
    const auto ub = this->bounds.second;
    auto size = lb.size();
    if (ub.size() > size)
    {
        size = ub.size();
    }

    auto x = std::vector<double>(size);
    for (int idx = 0; idx < size; ++idx)
    {
        std::uniform_real_distribution<double> uniform_distribution_bouded(lb[idx], ub[idx]);
        x[idx] = uniform_distribution_bouded(generator);
    }
    return x;
}

double FPA::getFitness(const Solution& solution) const
{
    // Make sum of the fitnesses of the solution.
    double fitness = 0;
    for (const auto& val : solution)
    {
        fitness += this->objectiveFunc->compute_fitness(val);
    }
    return fitness;
}


Solution FPA::amendSolution(const Solution& x, const std::pair<std::vector<double>, std::vector<double>>& bounds) const
{
    const auto possibleSolution = this->generateSolution();
    auto newSolution = Solution(x.size());

    for (int idx = 0; idx < x.size(); ++idx)
    {
        if (x[idx] < bounds.first[idx] || x[idx] > bounds.second[idx])
        {
            newSolution[idx] = possibleSolution[idx];
        }
        else
        {
            newSolution[idx] = x[idx];
        }
    }

    return newSolution;
}

Solution FPA::correctSolution(const Solution& x, const std::pair<std::vector<double>, std::vector<double>>& bounds) const
{
    const auto x_amended = this->amendSolution(x, bounds);

    auto x_new = std::vector<double>(x_amended.size());
    for (int idx = 0; idx < x_amended.size(); ++idx)
    {
        if (x_amended[idx] < bounds.first[idx])
        {
            x_new[idx] = bounds.first[idx];
        }
        else if (x_amended[idx] > bounds.second[idx])
        {
            x_new[idx] = bounds.second[idx];
        }
        else
        {
            x_new[idx] = x_amended[idx];
        }
    }
    return x_new;
}
