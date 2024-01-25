//
// Created by Robin RULLO on 23/01/2024.
//

#include "Logger.h"

#include <iomanip>
#include <sstream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"

void Logger::initializeLogger()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
    sinks.push_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            "/Users/rrullo/PrivateWorkspace/02__uha40/master/FPA/logs/log.txt", true));
    const auto combined_logger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));
    //register it if you need to access it globally
    spdlog::register_logger(combined_logger);
    const std::string jsonpattern =
        R"({"timestamp":"%Y-%m-%dT%H:%M:%S.%e%z","logger":"%n","log_level":"%l","process_id":%P,"thread_id":%t %v})";

    spdlog::set_pattern(jsonpattern);
}

std::shared_ptr<spdlog::logger> Logger::getLogger()
{
    return spdlog::get("logger");
}

char* Logger::vectorToJSON(std::vector<double> data)
{
    // Create a string stream to build the JSON array
    std::ostringstream oss;
    oss << '[';

    // Iterate through the vector
    for (size_t i = 0; i < data.size(); ++i)
    {
        // Add each double to the JSON array
        oss << data[i];

        // Add a comma if it's not the last element
        if (i < data.size() - 1)
        {
            oss << ',';
        }
    }

    // Close the JSON array
    oss << ']';

    // Convert the stringstream to a string
    const std::string jsonString = oss.str();

    // Allocate memory for the char* and copy the string contents
    const auto result = new char[jsonString.size() + 1];
    std::strcpy(result, jsonString.c_str());

    return result;
}

char* Logger::doubleToString(const double& d)
{
    std::ostringstream oss;
    oss << std::setprecision(17) << d;
    const auto string = oss.str();
    const auto result = new char[string.size() + 1];
    std::strcpy(result, string.c_str());

    return result;
}
