//
// Created by Robin RULLO on 23/01/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>

#include "spdlog/spdlog.h"


class Logger
{
public:
    static void initializeLogger();
    static std::shared_ptr<spdlog::logger> getLogger();
    static char* vectorToJSON(std::vector<double> vec);
    static char* doubleToString(const double& d);
};


#endif //LOGGER_H
