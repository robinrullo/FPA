//
// Created by Robin RULLO on 23/01/2024.
//

#ifndef AGENT_H
#define AGENT_H

#include <iomanip>
#include <vector>

using Solution = std::vector<double>;


inline int ID = 0;

class Agent {
public:
    Solution solution;
    double fitness;
    int id;

    explicit Agent(const std::vector<double>& solution = {}, double fitness = 0.);

    static int increase();
    Agent copy() const;
    void print() const;
};

#endif //AGENT_H
