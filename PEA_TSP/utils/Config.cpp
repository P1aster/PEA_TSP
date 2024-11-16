#include "Config.h"
#include <filesystem>

Config::Config(std::string configFilePath) {
    this->configFilePath = configFilePath;

}

bool Config::ensureExists(std::string path) {
    if (std::filesystem::exists(path)) {
        return true;
    }
    return false;
}

bool Config::checkPathIsFile(std::string path) {
    if (std::filesystem::is_regular_file(path)) {
        return true;
    }
    return false;
}


bool Config::createPath(std::string path) {
    try {
        // Create directories if they don't exist
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());

        // Create the file
        std::ofstream file(path);
        if (file) {
            file.close();
            return true;
        }
        else {
            std::cerr << "Failed to create file: " << path << std::endl;
            return false;
        }
    }
    catch (const std::filesystem::filesystem_error& error) {
        std::cerr << "Filesystem error: " << error.what() << std::endl;
        return false;
    }
}

bool Config::readConfig() {

    if (!ensureExists(configFilePath)) {
        std::cerr << "Config file does not exist and could not be created: " << configFilePath << std::endl;
        return false;
    }

    std::ifstream file(configFilePath);

    if (!file) {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        switch (lineNumber) {
        case 0:
            if (!ensureExists(line)) {
                std::cerr << "Input file or directory does not exist and could not be created: " << inputPath << std::endl;
                return false;
            }
            inputPath = line;
            break;
        case 1:
            if (!ensureExists(line) && !createPath(line)) {
                std::cerr << "Output file path does not exist and could not be created: " << outputPath << std::endl;
                return false;
            }
            outputPath = line;
            break;
        case 2:
            if (line == "all") {
                checkAllNodes = true;
            }
            else {
				checkAllNodes = false;
                std::istringstream iss(line);
                std::string token;
                while (iss >> token) {
                    try {
                        int number = std::stoi(token);
                        nodeList.push_back(number);
                    }
                    catch (const std::invalid_argument&) {
                        std::cerr << "Invalid argument: could not convert token to int: " << token << std::endl;
                        return false;
                    }
                }
            }
            break;
        case 3:
            try {
                repeatNumber = std::stoi(line);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument: could not convert line to int: " << line << std::endl;
                return false;
            }
            break;
        case 4:
            try {
                coutFlag = std::stoi(line); // convert string to int
            }
            catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument: could not convert line to int: " << line << std::endl;
                return false;
            }
            break;
        default:
            break;
        }
        lineNumber++;
    }

    file.close();
    return true;
}


std::string Config::getInputPath() { return inputPath; }

std::string Config::getOutputPath() { return outputPath; }

int Config::getRepeatNumber() { return repeatNumber; }

int Config::getCoutFlag() { return coutFlag; }

bool Config::getCheckAllNodes() { return checkAllNodes; }

std::vector<int> Config::getNodeList() { return nodeList; }

bool Config::openOutputFile() {
    outputFileOpt.emplace(outputPath); // try open file
    if (outputFileOpt->is_open()) {
        return true;
    } else {
        return false;
    }
}

void Config::writeToOutputFile(const std::string& content) {
    if (outputFileOpt && outputFileOpt->is_open()) {
        *outputFileOpt << content;
    }
    else {
        std::cerr << "Output file is not open." << std::endl;
    }
}

void Config::closeOutputFile() {
    if (outputFileOpt && outputFileOpt->is_open()) {
        outputFileOpt->close();
    }
}

void Config::cout(std::string str) {
    if (coutFlag == 1) {
        std::cout << str;
    }
}