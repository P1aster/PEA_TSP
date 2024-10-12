#include "ConfigReader.h"

ConfigReader::ConfigReader(std::string configFilePath) {
    this->configFilePath = configFilePath;

}

bool ConfigReader::ensureExists(std::string path) {
    if (std::filesystem::exists(path)) {
        return true;
    }
    return false;
}

bool ConfigReader::checkPathIsFile(std::string path) {
    if (std::filesystem::is_regular_file(path)) {
        return true;
    }
    return false;
}


bool ConfigReader::createPath(std::string path) {
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

bool ConfigReader::readConfig() {

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
            try {
                repeatNumber = std::stoi(line); // convert string to int
            } catch (const std::invalid_argument& error) {
                std::cerr << "Invalid argument: could not convert line to int: " << line << std::endl;
                return false;
            }
            break;
        case 3:
            try {
                coutFlag = std::stoi(line); // convert string to int
            }
            catch (const std::invalid_argument& error) {
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

std::string ConfigReader::getInputPath() { return inputPath; }

std::string ConfigReader::getOutputPath() { return outputPath; }

int ConfigReader::getRepeatNumber() { return repeatNumber; }

int ConfigReader::getCoutFlag() { return coutFlag; }

bool ConfigReader::openOutputFile() {
    outputFileOpt.emplace(outputPath); // try open file
    if (outputFileOpt->is_open()) {
        return true;
    } else {
        return false;
    }
}

void ConfigReader::writeToOutputFile(const std::string& content) {
    if (outputFileOpt && outputFileOpt->is_open()) {
        *outputFileOpt << content;
    }
    else {
        std::cerr << "Output file is not open." << std::endl;
    }
}

void ConfigReader::closeOutputFile() {
    if (outputFileOpt && outputFileOpt->is_open()) {
        outputFileOpt->close();
    }
}