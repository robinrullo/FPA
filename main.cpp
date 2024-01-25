#include <iostream>
#include <sstream>

#include "FPA.h"
#include "Logger.h"
#include "ObjectiveFunctions.h"

int main()
{
    clock_t tStart = clock();
    Logger::initializeLogger();

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

                    const auto fitness = Logger::doubleToString(bestAgent.fitness);
                    const auto solutions = Logger::vectorToJSON(bestAgent.solution);

                    Logger::getLogger()->info(
                        ",{}",
                        "\"compute_exec_time\": " +
                        std::to_string(static_cast<double>(clock() - tInnerStart) / CLOCKS_PER_SEC) +
                        ", \"dimensions\": " +
                        std::to_string(dimensions) + ", \"pop_size\": " + std::to_string(pop_size)
                        + ", \"objective_function\": \"" + objectiveFunc->getName() + "\", \"run\": " + std::to_string(run)
                        + ", \"fitness\": " + fitness + ", \"solutions\": " +
                        solutions);
                }
            }
        }
    }
    std::cout << "\nTemps d'execution global: " << static_cast<double>(clock() - tStart) / CLOCKS_PER_SEC << std::endl;
    return 1;
}
