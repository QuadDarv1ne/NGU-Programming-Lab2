#include "commands.h"
#include <iostream>
#include <algorithm>

// Глобальные структуры данных
std::map<std::string, std::vector<std::string>> trainSchedule;
std::map<std::string, std::set<std::string>> townTrains;

CommandType parseCommand(const std::string& commandStr) {
    if (commandStr == "CREATE_TRAIN") return CommandType::CREATE_TRAIN;
    if (commandStr == "TRAINS_FOR_TOWN") return CommandType::TRAINS_FOR_TOWN;
    if (commandStr == "TOWNS_FOR_TRAIN") return CommandType::TOWNS_FOR_TRAIN;
    if (commandStr == "TRAINS") return CommandType::TRAINS;
    return CommandType::UNKNOWN;
}

void processCreateTrain(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cout << "Incorrect command" << std::endl;
        return;
    }
    
    std::string trainName = tokens[0];
    std::vector<std::string> route(tokens.begin() + 1, tokens.end());
    
    // Удаление старого маршрута
    auto it = trainSchedule.find(trainName);
    if (it != trainSchedule.end()) {
        for (const auto& town : it->second) {
            townTrains[town].erase(trainName);
        }
    }
    
    // Добавление нового маршрута
    trainSchedule[trainName] = route;
    for (const auto& town : route) {
        townTrains[town].insert(trainName);
    }
}

void processTrainsForTown(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        std::cout << "Incorrect command" << std::endl;
        return;
    }
    
    std::string town = tokens[0];
    auto it = townTrains.find(town);
    
    if (it == townTrains.end() || it->second.empty()) {
        std::cout << "Trains is absent" << std::endl;
        return;
    }
    
    bool first = true;
    for (const auto& train : it->second) {
        if (!first) std::cout << " ";
        std::cout << train;
        first = false;
    }
    std::cout << std::endl;
}

void processTownsForTrain(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        std::cout << "Incorrect command" << std::endl;
        return;
    }
    
    std::string trainName = tokens[0];
    auto it = trainSchedule.find(trainName);
    
    if (it == trainSchedule.end()) {
        std::cout << "Train not found" << std::endl;
        return;
    }
    
    for (const auto& town : it->second) {
        std::cout << "Stop " << town << ": ";
        auto trains = townTrains[town];
        trains.erase(trainName);
        
        if (trains.empty()) {
            std::cout << "0";
        } else {
            bool first = true;
            for (const auto& t : trains) {
                if (!first) std::cout << " ";
                std::cout << t;
                first = false;
            }
        }
        std::cout << std::endl;
    }
}

void processTrains() {
    if (trainSchedule.empty()) {
        std::cout << "Trains is absent" << std::endl;
        return;
    }
    
    for (const auto& [train, route] : trainSchedule) {
        std::cout << "TRAIN " << train << ":";
        for (const auto& town : route) {
            std::cout << " " << town;
        }
        std::cout << std::endl;
    }
}
