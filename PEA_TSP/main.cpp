#include <iostream>
#include "Graph.h"
#include "BF.h"
#include "RNN.h"
#include "ConfigReader.h"
#include "Time.h"

int main(int argslen, char* args[]) {
    Graph graph;
    Time timer;
    TSP_Result result;
    std::string inputPath;
    std::string outputPath;
    bool isFileOpended = false;


    if (argslen == 0) {
        std::cerr << "No arguments provided." << std::endl;
        return 1;
    }

    ConfigReader configReader(args[1]);
    if (!configReader.readConfig()) {
        return 1;
    }
    inputPath = configReader.getInputPath();

    if (configReader.checkPathIsFile(inputPath)) {
        if (graph.loadFromFile(inputPath)) {
            std::cout << "Loaded graph from file: " << inputPath << "\n\n";
            graph.printMatrix();
        }
        else {
            std::cerr << "Failed to load graph from file: " << inputPath << std::endl;
            return 1;
        }

        isFileOpended = configReader.openOutputFile();
        if (!isFileOpended) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            return 1;
        }

        BF bf(graph);

        configReader.writeToOutputFile("Instance,Brute force algorithm\n");
        configReader.writeToOutputFile("Input Path," + inputPath + "\n");
        configReader.writeToOutputFile("Nodes Number," + std::to_string(graph.getNodesNumber()) + "\n");
        configReader.writeToOutputFile("Time,[ms]\n");

        for (int i = 0; i < configReader.getRepeatNumber(); i++) {
            timer.start();
            result = bf.findCheapestHamiltonianCircle();
            timer.stop();
            configReader.writeToOutputFile(std::to_string(i+1)+ "," + std::to_string(timer.getElapsedTime()) + "\n");
        }

        std::cout << std::endl;
    }
    else {

        isFileOpended = configReader.openOutputFile();
        if (!isFileOpended) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            return 1;
        }


        configReader.writeToOutputFile("Instance,Brute force algorithm\n");


        for (const auto& entry : std::filesystem::directory_iterator(inputPath)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                std::cout << "Processing file: " << filePath << std::endl;
                if (graph.loadFromFile(filePath)) {
                    std::cout << "Loaded graph from file: " << filePath << "\n\n";
                    graph.printMatrix();
                    std::cout << std::endl;
                }
                else {
                    std::cerr << "Failed to load graph from file: " << filePath << std::endl;
                    return 1;
                }
                BF bf(graph);

                configReader.writeToOutputFile("Input Path," + filePath + "\n");
                configReader.writeToOutputFile("Nodes Number," + std::to_string(graph.getNodesNumber()) + "\n");
                configReader.writeToOutputFile("Time,[ms]\n");

                for (int i = 0; i < configReader.getRepeatNumber(); i++) {
                    timer.start();
                    result = bf.findCheapestHamiltonianCircle();
                    timer.stop();
                    configReader.writeToOutputFile(std::to_string(i + 1) + "," + std::to_string(timer.getElapsedTime()) + "\n");
                }

                configReader.writeToOutputFile("\n\n");
            }
        }
    }
    return 0;
}