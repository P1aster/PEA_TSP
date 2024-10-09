#include <iostream>
#include "Graph.h"
#include "BF.h"
#include "RNN.h"
#include "ConfigReader.h"

int main(int argslen, char* args[]) {
    Graph graph;
    std::string inputPath;


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
            std::cout << "Loaded graph from file: " << inputPath << std::endl;
            graph.printMatrix();
        }
        else {
            std::cerr << "Failed to load graph from file: " << inputPath << std::endl;
            return 1;
        }

        std::cout << std::endl << "Brute force algorithm:" << "\n\n";

        BF bf(graph);
        RNN rnn(graph);

        TSP_Result resultRNN = rnn.findRepeatedNearestNaighbour();
        TSP_Result resultNN = rnn.findNearestNaighbour();
        TSP_Result resultBF = bf.findCheapestHamiltonianCircle();


        std::cout << "Best path RNN: " << resultRNN.minPathCost << "   ";
        for (size_t i = 0; i < resultRNN.bestPath.size(); ++i) {
            std::cout << resultRNN.bestPath[i] << " ";
        }
        std::cout << "\n\n\n";

        std::cout << "Best path NN: " << resultNN.minPathCost << "   ";
        for (size_t i = 0; i < resultNN.bestPath.size(); ++i) {
            std::cout << resultNN.bestPath[i] << " ";
        }
        std::cout << "\n\n\n";

        std::cout << "Best path BF: " << resultBF.minPathCost << "   ";
        for (size_t i = 0; i < resultBF.bestPath.size(); ++i) {
            std::cout << resultBF.bestPath[i] << " ";
        }
        std::cout << "\n\n\n";

    }
    else {
        for (const auto& entry : std::filesystem::directory_iterator(inputPath)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                std::cout << "Processing file: " << filePath << std::endl;
                if (graph.loadFromFile(filePath)) {
                    graph.printMatrix();
                }
                else {
                    std::cerr << "Failed to load graph from file: " << filePath << std::endl;
                }
            }
        }
    }

    
    return 0;
}