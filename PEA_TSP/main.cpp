#include <iostream>
#include "algorithms/BF.h"
#include "algorithms/BB.h"
#include "algorithms/R.h"
#include "algorithms/RNN.h"
#include "utils/Config.h"
#include "utils/Time.h"
#include "structures/Graph.h"
#include <queue>
#include "ErrorCalculator.h"

void processBFGraph(Graph& graph, Config& config, const std::vector<int>& nodesList, int knownMinPathCost) {
    Time timer;
    ErrorCalculator errorCalculator;
    BF bf(graph);
    double totalElapsedTime = 0;
    double totalAbsoluteError = 0;
    double totalRelativeErrorPercentage = 0;
    int repeats = config.getRepeatNumber();
    for (int node : nodesList) {
        for (int repeat = 0; repeat < repeats; repeat++) {
            timer.start();
            TSP_Result result = bf.findCheapestHamiltonianCircle(node);
            timer.stop();

            config.cout("\n--BF--");
            if (knownMinPathCost != INT_MAX) {
                config.cout("\nKnown min path cost: " + knownMinPathCost);
            }
            config.cout("\nMin found path cost: " + std::to_string(result.minPathCost));
            config.cout("\nBest path: \n");

            for (int i = 0; i < result.bestPath.size(); i++) {
                int res = result.bestPath[i];
                config.cout(std::to_string(res) + " ");
            }
            float elapsedTime = timer.getElapsedTime();
            config.cout("\nExecution time: " + std::to_string(elapsedTime) + " ms\n");

            config.writeToOutputFile(std::to_string(elapsedTime) + ",");

            totalElapsedTime += elapsedTime;

            if (knownMinPathCost != INT_MAX) {
                errorCalculator.setTrueValue(knownMinPathCost);
                double absoluteError = errorCalculator.calculateAbsoluteError(result.minPathCost);
                double relativeErrorPercentage = errorCalculator.calculateRelativeError(result.minPathCost);
                config.cout("\nAbsolute Error: " + std::to_string(absoluteError));
                config.cout("\nRelative Error: " + std::to_string(relativeErrorPercentage) + "%\n\n");

                totalAbsoluteError += absoluteError;
                totalRelativeErrorPercentage += relativeErrorPercentage;
            }
        }
    }

    double averageElapsedTime = totalElapsedTime / repeats;
    double averageAbsoluteError = totalAbsoluteError / repeats;
    double averageRelativeErrorPercentage = totalRelativeErrorPercentage / repeats;

    config.writeToOutputFile(std::to_string(averageElapsedTime) + ",");
    config.writeToOutputFile(std::to_string(averageAbsoluteError) + ",");
    config.writeToOutputFile(std::to_string(averageRelativeErrorPercentage) + "%\n");
}

void processRNNGraph(Graph& graph, Config& config, const std::vector<int>& nodesList, int knownMinPathCost) {
    Time timer;
    ErrorCalculator errorCalculator;
    RNN rnn(graph);
    double totalElapsedTime = 0;
    double totalAbsoluteError = 0;
    double totalRelativeErrorPercentage = 0;
	int repeats = config.getRepeatNumber();
    
    for (int repeat = 0; repeat < repeats; repeat++) {
        timer.start();
        TSP_Result result = rnn.findRepeatedNearestNaighbour();
        timer.stop();

        config.cout("\n--NN--");
        if (knownMinPathCost != INT_MAX) {
            config.cout("\nKnown min path cost: " + knownMinPathCost);
        }
        config.cout("\nMin found path cost: " + std::to_string(result.minPathCost));
        config.cout("\nBest path: \n");

        for (int i = 0; i < result.bestPath.size(); i++) {
            int res = result.bestPath[i];
            config.cout(std::to_string(res) + " ");
        }
        float elapsedTime = timer.getElapsedTime();
        config.cout("\nExecution time: " + std::to_string(elapsedTime) + " ms\n");

        config.writeToOutputFile(std::to_string(elapsedTime) + ",");

        totalElapsedTime += elapsedTime;

        if (knownMinPathCost != INT_MAX) {
            errorCalculator.setTrueValue(knownMinPathCost);
            double absoluteError = errorCalculator.calculateAbsoluteError(result.minPathCost);
            double relativeErrorPercentage = errorCalculator.calculateRelativeError(result.minPathCost);
            config.cout("\nAbsolute Error: " + std::to_string(absoluteError));
            config.cout("\nRelative Error: " + std::to_string(relativeErrorPercentage) + "%\n\n");

            totalAbsoluteError += absoluteError;
            totalRelativeErrorPercentage += relativeErrorPercentage;
        }
    }

    double averageElapsedTime = totalElapsedTime / repeats;
    double averageAbsoluteError = totalAbsoluteError / repeats;
    double averageRelativeErrorPercentage = totalRelativeErrorPercentage / repeats;

    config.writeToOutputFile(std::to_string(averageElapsedTime) + ",");
    config.writeToOutputFile(std::to_string(averageAbsoluteError) + ",");
    config.writeToOutputFile(std::to_string(averageRelativeErrorPercentage) + "%\n");
}

int main(int argslen, char* args[]) {
    Graph graph;
    ProcessFileResult fileRes;
    std::string inputPath;
    std::string outputPath;
    bool isFileOpended = false;

    if (argslen == 1) {
        std::cerr << "No arguments provided." << std::endl;
        return 1;
    }

    Config config(args[1]);
    if (!config.readConfig()) {
        return 1;
    }

    inputPath = config.getInputPath();

    if (config.checkPathIsFile(inputPath)) {

        isFileOpended = config.openOutputFile();
        if (!isFileOpended) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            return 1;
        }

        fileRes = config.preprocessOutputFile(inputPath, graph);

        #ifdef NN
            processRNNGraph(graph, config, fileRes.nodesList, fileRes.knownMinPathCost);
        #elif BNN
            std::cout << "JD!" << std::endl;
        #else
            std::cout << "No function is defined to call!" << std::endl;
        #endif

    }
    else {
        isFileOpended = config.openOutputFile();
        if (!isFileOpended) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            return 1;
        }

        for (const auto& entry : std::filesystem::directory_iterator(inputPath)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                std::cout << "Processing file: " << filePath << std::endl;



            }
        }
    }
    return 0;
}