//
// Created by Robin RULLO on 23/01/2024.
//

#ifndef FPA_H
#define FPA_H

#include <vector>

#include "Agent.h"
#include "ObjectiveFunctions.h"


class FPA
{
public:
    FPA(
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
    );
    void evolve(const double& epoch);
    std::pair<std::vector<std::pair<Agent, int>>, Agent> updateGlobalBestAgent(const std::vector<Agent>& pop);
    static std::vector<std::pair<Agent, int>> getSortedPopulation(const std::vector<Agent>& pop, const std::string& minmax);
    static Agent getBetterAgent(const std::pair<Agent, Agent>& agents, const std::string& minmax, const bool& reverse);
    std::vector<Agent> generatePopulation() const;
    Solution generateSolution() const;
    [[nodiscard]] double getFitness(const Solution& solution) const;
    Solution amendSolution(const Solution& x, const std::pair<std::vector<double>, std::vector<double>>& bounds) const;
    Solution correctSolution(const Solution& x, const std::pair<std::vector<double>, std::vector<double>>& bounds) const;
    Agent& solve();

private:
    int epoch{};
    int pop_size{};
    double pc{};
    double pm{};
    double p_s{};
    double levy_multiplier{};
    const ObjectiveFunction* objectiveFunc;
    std::pair<std::vector<double>, std::vector<double>> bounds;
    std::string minmax;
    int dimensions;
    std::vector<Agent> pop;
    Agent g_best;
};


#endif //FPA_H
