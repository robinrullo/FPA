#include <iostream>
#include <sstream>

#include "FPA.h"
#include "ObjectiveFunctions.h"

int main()
{
    clock_t tStart = clock();

    std::vector<ObjectiveFunction*> objectiveFunctions = {
        new Sphere(), new Rosenbrock(), new Ackley(), new Rastrigin()
    };
    for (auto& objectiveFunc : objectiveFunctions)
    {
        for (auto& dimensions : {10, 30, 50})
        {
            for (auto& pop_size : {30, 50, 70})
            {
                for (int run = 0; run < 10; ++run)
                {
                    clock_t tInnerStart = clock();
                    // Gen bounds
                    auto lb = std::vector(dimensions, -100.);
                    auto ub = std::vector(dimensions, 100.);
                    auto bounds = std::make_pair(lb, ub);
                    std::string minmax = "min";
                    auto epoch = 5000;
                    auto pc = 0.85;
                    auto pm = 0.1;
                    auto p_s = 0.8;
                    auto levy_multiplier = 0.2;

                    auto optimizer = FPA(epoch, pop_size, pc, pm, p_s, levy_multiplier, objectiveFunc, bounds, minmax,
                                         dimensions);
                    auto bestAgent = optimizer.solve();

                    // Join vector to string
                    auto ss = std::stringstream();
                    for (auto& x : bestAgent.solution)
                    {
                        ss << x << " ";
                    }
                    auto solution = ss.str();

                    std::cout << "Agent: " << "Fitness: " << bestAgent.fitness << " Solutions: " << solution << std::endl;
                    std::cout << "Fonction objectif: " << objectiveFunc->getName() << std::endl;
                    std::cout << "Dimensions: " << dimensions << std::endl;
                    std::cout << "Taille de la population: " << pop_size << std::endl;
                    std::cout << "Run: " << run << std::endl;
                    std::cout << "Temps d'execution: " << static_cast<double>(clock() - tInnerStart) / CLOCKS_PER_SEC << std::endl;
                    // Separator
                    std::cout << "----------------------------------------" << std::endl;
                }
            }
        }
    }
    std::cout << "\nTemps d'execution global: " << static_cast<double>(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
    return 1;
}
