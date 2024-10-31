#include <iostream>
#include "algorithms/BF.h"
#include "algorithms/BB.h"
#include "algorithms/R.h"
#include "algorithms/RNN.h"
#include "utils/Config.h"
#include "utils/Time.h"
#include "structures/Graph.h"
#include <queue>






int main(int argslen, char* args[]) {
    Graph graph;

    Time timer;
    TSP_Result result;
    std::string inputPath;
    std::string outputPath;
    int repeats;
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
    repeats = config.getRepeatNumber();



    if (config.checkPathIsFile(inputPath)) {
        if (graph.loadFromFile(inputPath)) {
            std::cout << "Loaded graph from file: " << inputPath << "\n\n";
            graph.printMatrix();
        }
        else {
            std::cerr << "Failed to load graph from file: " << inputPath << std::endl;
            return 1;
        }

        isFileOpended = config.openOutputFile();
        if (!isFileOpended) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            return 1;
        }

    

        config.writeToOutputFile("Instance,Brute force algorithm\n");
        config.writeToOutputFile("Input Path," + inputPath + "\n");
        config.writeToOutputFile("Nodes Number," + std::to_string(graph.getNodesNumber()) + "\n");
        config.writeToOutputFile("Time,[ms]\n");


        BB bb(graph);
        BF bf(graph);
		RNN rnn(graph);
        R r(graph);


        /*timer.start();
        result = r.findRandomHamiltonianCircle();
        timer.stop();

        std::cout << "R: Min path cost: " << result.minPathCost << "\n";
        std::cout << "R: Best path: ";
        std::cout << timer.getElapsedTime() << " ms\n";
        for (int i = 0; i < result.bestPath.size(); i++) {
            std::cout << result.bestPath[i] << " ";
        }
        std::cout << "\n\n";*/


        timer.start();
        result = bf.findCheapestHamiltonianCircle(0);
        timer.stop();

        std::cout << "BF: Min path cost: " << result.minPathCost << "\n";
        std::cout << "BF: Best path: ";
        std::cout << timer.getElapsedTime() << " ms\n";
        for (int i = 0; i < result.bestPath.size(); i++) {
            std::cout << result.bestPath[i] << " ";
        }
        std::cout << "\n\n";


        timer.start();
        result = rnn.findNearestNaighbour(0);
        timer.stop();

        std::cout << "NN: Min path cost: " << result.minPathCost << "\n";
        std::cout << "NN: Best path: ";
        std::cout << timer.getElapsedTime() << " ms\n";
        for (int i = 0; i < result.bestPath.size(); i++) {
            std::cout << result.bestPath[i] << " ";
        }
        std::cout << "\n\n";


        timer.start();
        result = rnn.findRepeatedNearestNaighbour();
        timer.stop();

        std::cout << "RNN: Min path cost: " << result.minPathCost << "\n";
        std::cout << "RNN: Best path: ";
        std::cout << timer.getElapsedTime() << " ms\n";
        for (int i = 0; i < result.bestPath.size(); i++) {
            std::cout << result.bestPath[i] << " ";
        }
        std::cout << "\n\n";



        for (int i = 0; i < 6; i++) {

            timer.start();
            result = bb.findCheapestHamiltonianCircle_BFS(i);
            timer.stop();

            std::cout << "BB_BFS: Min path cost: " << result.minPathCost << "\n";
            std::cout << "BB_BFS: Best path: ";
            std::cout << timer.getElapsedTime() << " ms\n";
            for (int i = 0; i < result.bestPath.size(); i++) {
                std::cout << result.bestPath[i] << " ";
            }
            std::cout << "\n\n";
        }


      /*  for (int i = 0; i < repeats; i++) {

            config.cout("Processing instance: " + std::to_string(i + 1) + " [begin] \n");

            timer.start();
            result = bf.findCheapestHamiltonianCircle(0);
            timer.stop();
            config.writeToOutputFile(std::to_string(i+1)+ "," + std::to_string(timer.getElapsedTime()) + "\n");

            config.cout("Processing instance: " + std::to_string(i + 1) + " [end] \n");
        }*/

        std::cout << std::endl;
    }
    else {

        isFileOpended = config.openOutputFile();
        if (!isFileOpended) {
            std::cerr << "Failed to open output file: " << outputPath << std::endl;
            return 1;
        }


        config.writeToOutputFile("Instance,Brute force algorithm\n");


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

                config.writeToOutputFile("Input Path," + filePath + "\n");
                config.writeToOutputFile("Nodes Number," + std::to_string(graph.getNodesNumber()) + "\n");
                config.writeToOutputFile("Time,[ms]\n");


                for (int i = 0; i < repeats; i++) {

                    config.cout("Processing instance: " + std::to_string(i + 1) + " [begin] \n");

                    timer.start();
                    result = bf.findCheapestHamiltonianCircle(0);
                    timer.stop();

                    config.writeToOutputFile(std::to_string(i + 1) + "," + std::to_string(timer.getElapsedTime()) + "\n");

                    config.cout("Processing instance: " + std::to_string(i + 1) + " [end] \n");

                }

                config.writeToOutputFile("\n\n");
            }
        }
    }
    return 0;
}