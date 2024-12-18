#include <iostream>
#include "algorithms/BF.h"
#include "algorithms/BB.h"
#include "algorithms/R.h"
#include "algorithms/RNN.h"
#include "utils/Config.h"
#include "utils/Time.h"
#include "utils/ErrorCalculator.h"
#include "structures/Graph.h"
#include <queue>
#include <functional>
#include "SA.h"
#include "CoolingSchema.h"



void processGraph(
    Graph& graph,
    Config& config,
    std::optional<unsigned int> knownMinPathCost,
    std::function<TSP_Result()> algorithmFunction,
    const std::string& algorithmName
) {
    Time timer;
    ErrorCalculator errorCalculator;
    double totalElapsedTime = 0;
    double totalAbsoluteError = 0;
    double totalRelativeErrorPercentage = 0;
    int repeats = config.getRepeatNumber();

    TSP_Result bestResult;
    bool firstRun = true;

    config.writeToOutputFile(",");
    for (int repeat = 0; repeat < repeats; repeat++) {
        timer.start();
        TSP_Result result = algorithmFunction();
        timer.stop();
        if (firstRun || result.minPathCost < bestResult.minPathCost) {
            bestResult = result;
            firstRun = false;
        }

        config.cout("\n--" + algorithmName + "--");
        if (knownMinPathCost.has_value()) {
            config.cout("\nKnown min path cost: " + std::to_string(knownMinPathCost.value()));
        }
        config.cout("\nMin found path cost: " + std::to_string(result.minPathCost));
        config.cout("\nBest path: \n");
        for (int node : result.bestPath) {
            config.cout(std::to_string(node) + " ");
        }

        float elapsedTime = timer.getElapsedTime();
        config.cout("\nExecution time: " + std::to_string(elapsedTime) + " ms\n");
        config.writeToOutputFile(std::to_string(elapsedTime) + ",");
        totalElapsedTime += elapsedTime;

        if (knownMinPathCost.has_value()) {
            errorCalculator.setTrueValue(knownMinPathCost.value());
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

    int bestMinPathCost = bestResult.minPathCost;

    // Write the final results including the best path cost found
    config.writeToOutputFile(std::to_string(averageElapsedTime) + ",");
    config.writeToOutputFile(std::to_string(bestMinPathCost) + ",");
    config.writeToOutputFile(std::to_string(averageAbsoluteError) + ",");
    config.writeToOutputFile(std::to_string(averageRelativeErrorPercentage) + "%\n");
}

void processGraphWithStartNodes(
    Graph& graph,
    Config& config,
    const std::vector<int>& nodesList,
    std::optional<unsigned int> knownMinPathCost,
    std::function<TSP_Result(int)> algorithmFunction,
    const std::string& algorithmName
) {
    Time timer;
    ErrorCalculator errorCalculator;

    // For each starting node
    for (int startNode : nodesList) {
        double totalElapsedTime = 0;
        double totalAbsoluteError = 0;
        double totalRelativeErrorPercentage = 0;
        TSP_Result bestResult;
        bool firstRun = true;

        config.writeToOutputFile(std::to_string(startNode) + ",");

        int repeats = config.getRepeatNumber();

        // Perform repeated measurements for each starting node
        for (int repeat = 0; repeat < repeats; repeat++) {
            timer.start();
            TSP_Result result = algorithmFunction(startNode);
            timer.stop();

            // Update best result if this is the first run or if we found a better path
            if (firstRun || result.minPathCost < bestResult.minPathCost) {
                bestResult = result;
                firstRun = false;
            }

            float elapsedTime = timer.getElapsedTime();
            totalElapsedTime += elapsedTime;

            // Output detailed information only for the first repeat
            if (repeat == 0) {
                config.cout("\n--" + algorithmName + " (Start Node: " + std::to_string(startNode) + ")--");
                if (knownMinPathCost.has_value()) {
                    config.cout("\nKnown min path cost: " + std::to_string(knownMinPathCost.value()));
                }
                config.cout("\nMin found path cost: " + std::to_string(result.minPathCost));
                config.cout("\nBest path: \n");
                for (int node : result.bestPath) {
                    config.cout(std::to_string(node) + " ");
                }
                config.cout("\nExecution time: " + std::to_string(elapsedTime) + " ms\n");

            }

            config.writeToOutputFile(std::to_string(elapsedTime) + ",");
        }

        double averageElapsedTime = totalElapsedTime / repeats;

        // Write the results including the best path cost found for this start node
        config.writeToOutputFile(std::to_string(averageElapsedTime) + ",");
        config.writeToOutputFile(std::to_string(bestResult.minPathCost) + ",");

        // Calculate and write error metrics if known min path cost is available
        if (knownMinPathCost.has_value()) {
            errorCalculator.setTrueValue(knownMinPathCost.value());
            double absoluteError = errorCalculator.calculateAbsoluteError(bestResult.minPathCost);
            double relativeErrorPercentage = errorCalculator.calculateRelativeError(bestResult.minPathCost);

            config.writeToOutputFile(std::to_string(absoluteError) + ",");
            config.writeToOutputFile(std::to_string(relativeErrorPercentage) + "%");
        }

        config.writeToOutputFile("\n");
    }
}
void processBF(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BF bf(graph);

    auto algorithmFunction = [&bf](int startNode) {
        return bf.findCheapestHamiltonianCircle(startNode);
        };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BF");
}

void processRNN(Graph& graph, Config& config, std::optional<int> knownMinPathCost) {
    RNN rnn(graph);
    auto algorithmFunction = [&rnn]() {
        return rnn.findRepeatedNearestNaighbour();
        };

    processGraph(graph, config, knownMinPathCost, algorithmFunction, "RNN");
}

void processBRNN(Graph& graph, Config& config, std::optional<int> knownMinPathCost) {
	RNN rnn(graph);
    
    auto algorithmFunction = [&rnn]() {
        return rnn.findBestRepeatedNearestNeighbour();
    };

    processGraph(graph, config, knownMinPathCost, algorithmFunction, "BRNN");

}

void processR(Graph& graph, Config& config, std::optional<unsigned int> knownMinPathCost, std::optional<unsigned int> permutations, std::optional<unsigned int> maxDuration) {
    R r(graph);

    config.writeToOutputFile(",");
    auto algorithmFunction = [&r, permutations, knownMinPathCost, maxDuration]() {
        return r.findBestRandomHamiltonianCircle(permutations, knownMinPathCost, maxDuration);
        };

    processGraph(graph, config, knownMinPathCost.value_or(INT_MAX), algorithmFunction, "R");
}

void processBBDFS(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BB bb(graph);

    auto algorithmFunction = [&bb](int startNode) {
        return bb.findCheapestHamiltonianCircle_DFS(startNode);
        };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BB_DFS");
}

void processBBLC(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BB bb(graph);

    auto algorithmFunction = [&bb](int startNode) {
        return bb.findCheapestHamiltonianCircle_LC(startNode);
        };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BB_LC");
}

void processBFS(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BB bb(graph);

    auto algorithmFunction = [&bb](int startNode) {
        return bb.findCheapestHamiltonianCircle_BFS(startNode);
        };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BB_BFS");
}

void processBRNNBBDFS(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BB bb(graph);
    RNN rnn(graph);
    auto algorithmFunction = [&bb, &rnn](int startNode) {
        TSP_Result result = rnn.findBestRepeatedNearestNeighbour();
        return bb.findCheapestHamiltonianCircle_DFS(startNode, result.minPathCost);
        };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BB_DFS");
}

void processBRNNBBLC(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BB bb(graph);
    RNN rnn(graph);
    auto algorithmFunction = [&bb, &rnn](int startNode) {
        TSP_Result result = rnn.findBestRepeatedNearestNeighbour();
        return bb.findCheapestHamiltonianCircle_LC(startNode, result.minPathCost);
        };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BB_DFS");
}

void processBRNNBBBFS(Graph& graph, Config& config, const std::vector<int>& nodesList, std::optional<int> knownMinPathCost) {
    BB bb(graph);
    RNN rnn(graph);
    auto algorithmFunction = [&bb, &rnn](int startNode) {
        TSP_Result result = rnn.findBestRepeatedNearestNeighbour();
        return bb.findCheapestHamiltonianCircle_BFS(startNode, result.minPathCost);
    };

    processGraphWithStartNodes(graph, config, nodesList, knownMinPathCost, algorithmFunction, "BB_BRNN_BFS");
}

void processSA(Graph& graph, Config& config, std::optional<int> knownMinPathCost) {
	SA sa(graph, CoolingSchema::Exponential);

    double initTemp = config.getInitialTemperature();
    double finalTemp = config.getFinalTemperature();
    double coolingR = config.getCoolingRate();

	auto algorithmFunction = [&sa, initTemp, finalTemp, coolingR]() {
		return sa.run(initTemp, finalTemp, coolingR);
		};
	processGraph(graph, config, knownMinPathCost, algorithmFunction, "SA");
}

int main(int argslen, char* args[]) {
    Graph graph;

    if (argslen == 1) {
        std::cerr << "No arguments provided." << std::endl;
        return 1;
    }

    Config config(args[1]);
    if (!config.readConfig()) {
        return 1;
    }

    std::string filePath = config.getInputPath();

    if (graph.loadFromFile(filePath)) {
        std::cout << "Loaded graph from file: " << filePath << "\n\n";
        graph.printMatrix();
        std::cout << std::endl;
    }
    else {
        std::cerr << "Failed to load graph from file: " << filePath << std::endl;
    }

    int nodesNumber = graph.getNodesNumber();
    std::optional<int> knownMinPathCost = config.getKnownMinPathCost();
	std::optional<int> permutations = config.getPermutations();
	std::optional<int> maxDuration = config.getMaxDuration();

    std::vector<int> nodesList;

    if (config.getCheckAllNodes()) {
        nodesList = std::vector<int>(nodesNumber);
        for (int i = 0; i < nodesNumber; ++i) {
            nodesList[i] = i;
        }
    } else {
		nodesList = config.getNodeList();
    }

    if (!config.openOutputFile()) {
		std::cerr << "Failed to open output file." << std::endl;
		return 1;
    }

    config.preprocessOutputFile(knownMinPathCost, nodesNumber);

    #ifdef BUILD_BF
        processBF(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_RNN)
        processRNN(graph, config, knownMinPathCost);
    #elif defined(BUILD_BRNN)
	    processBRNN(graph, config, knownMinPathCost);
    #elif defined(BUILD_R)
	    processR(graph, config, knownMinPathCost, permutations, maxDuration);
    #elif defined(BUILD_BB_DFS)
	    processBBDFS(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_BB_LC)
	    processBBLC(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_BB_BFS)
        processBFS(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_BRNN_BB_DFS)
        processBRNNBBDFS(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_BRNN_BB_LC)
        processBRNNBBLC(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_BRNN_BB_BFS)
        processBRNNBBBFS(graph, config, nodesList, knownMinPathCost);
    #elif defined(BUILD_SA)
        processSA(graph, config, knownMinPathCost);
    #else
        std::cout << "No function is defined to call!" << std::endl;
        return 1;
    #endif

	config.closeOutputFile();
    return 0;
}