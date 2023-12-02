#include "utils.h"

std::tm convertToDate(const std::string& str) {
    // std::tm date = {};
    // if (strptime(str.c_str(), "%Y-%m-%d", &date) == nullptr) {
    //     throw std::invalid_argument("Invalid date format");
    // }
    // return date;

    std::tm date = {};
    std::istringstream ss(str);
    ss >> std::get_time(&date, "%Y-%m-%d");
    if (ss.fail()) {
        throw std::runtime_error("Failed to convert string to date.");
    }
    return date;
}

double convertToDouble(const std::string& str) {
    try {
        return std::stod(str);
    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Invalid double format");
    }
}

long convertToLong(const std::string& str) {
    try {
        return std::stol(str);
    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Invalid long format");
    }
}