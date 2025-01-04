#include "Genetic.h"
#include <algorithm>
#include "algorithms/R.h"
#include "utils/Time.h"


Genetic::Genetic(Graph graph) {
    this->graph = graph;
    this->matrix = graph.getMatrix();
    this->nodesNumber = graph.getNodesNumber();
    this->bestSolution = Path();

    std::mt19937 gen(rd());
}


Path Genetic::generateRandomPath() {
    R r(graph);
    TSP_Result result = r.findFirstRandomHamiltonianCircle();

	Path path;
	path.setNodesList(result.bestPath);
    path.setCost(result.minPathCost);

	return path;
}

// Generate initial population
std::vector<Path> Genetic::generateInitialPopulation(int populationSize) {
    std::vector<Path> population;
    
    for (int i = 0; i < populationSize; i++) {
        population.push_back(generateRandomPath());
    }
    return population;
}

// Swap mutation (reciprocal exchange)
Path Genetic::mutate_swap(Path path) {
    std::uniform_int_distribution<> dis(0, nodesNumber - 1);
    std::vector<int> nodes = path.getNodesList();

    int pos1 = dis(gen);
    int pos2 = dis(gen);

    std::swap(nodes[pos1], nodes[pos2]);
    path.setNodesList(nodes);
    path.calculateCost(graph);

    return path;
}
// Inversion mutation
Path Genetic::mutate_invert(Path path) {
    std::uniform_int_distribution<> dis(0, nodesNumber - 1);
    std::vector<int> nodes = path.getNodesList();

    int start = dis(gen);
    int end = dis(gen);

    if (start > end) {
        std::swap(start, end);
    }

    std::reverse(nodes.begin() + start, nodes.begin() + end + 1);
    path.setNodesList(nodes);
    path.calculateCost(graph);

    return path;
}

// Tournament selection
Path Genetic::selection_turnament(const std::vector<Path>& population) {

    int N = population.size();

    // Generate random tournament size r where 2 ≤ r < N
    std::uniform_int_distribution<> sizeDis(2, N - 1);
    int r = sizeDis(gen);

    // Select r random individuals for tournament
    std::uniform_int_distribution<> dis(0, N - 1);

    Path best = population[dis(gen)];
    for (int i = 1; i < r; i++) {
        Path contestant = population[dis(gen)];
        if (contestant.getCost() < best.getCost()) {
            best = contestant;
        }
    }
    return best;
}

// Order Crossover (OX)
Path Genetic::crossover(const Path& parent1, const Path& parent2) {
    Path child(nodesNumber);
    std::vector<int> childPath(nodesNumber, -1);

    // Select random subsequence from parent1
    std::uniform_int_distribution<> dis(0, nodesNumber - 1);
    int start = dis(gen);
    int end = dis(gen);

    if (start > end) {
        std::swap(start, end);
    }

    // Copy subsequence from parent1
    std::vector<bool> used(nodesNumber, false);
    for (int i = start; i <= end; i++) {
        childPath[i] = parent1.getNodesList()[i];
        used[childPath[i]] = true;
    }

    // Fill remaining positions with cities from parent2 in order
    int j = (end + 1) % nodesNumber;
    for (int i = 0; i < nodesNumber; i++) {
        int city = parent2.getNodesList()[i];
        if (!used[city]) {
            while (childPath[j] != -1) {
                j = (j + 1) % nodesNumber;
            }
            childPath[j] = city;
            used[city] = true;
        }
    }

    child.setNodesList(childPath);
    child.calculateCost(graph);
    return child;
}

// Main genetic algorithm function
void Genetic::solve(int mu, int lambda, int generations, double mutationRate) {

    if (mu >= lambda) {
        throw std::invalid_argument("Lambda must be greater than mu in (μ,λ) evolution strategy");
    }
	int iterations = 0;

    // Initialize timer
    Time timer;
    timer.start();

    // Generate initial parent population of size μ
    std::vector<Path> parents = generateInitialPopulation(mu);

    // Track best solution ever found
    bestSolution = parents[0];

    for (const Path& p : parents) {
        if (p.getCost() < bestSolution.getCost()) {
            bestSolution = p;
        }
    }


    // Evolution loop
   while(true) {

	   iterations++;
        // Check time-based stopping criterion
        if (maxDurationMs.has_value() && timer.getElapsedTimeNow() >= maxDurationMs.value()) {
            break;
        }

        // Check if we found solution with known minimum cost
        if (knownMinPathCost.has_value() && bestSolution.getCost() <= knownMinPathCost.value()) {
            break;
        }

		// Check generation-based stopping criterion
		if (generations != -1 && iterations >= generations) {
			break;
		}

        // Generate λ offspring
        std::vector<Path> offspring;

        while (offspring.size() < lambda) {
            // Select parents using tournament selection
            Path parent1, parent2;
            parent1 = selection_turnament(parents);
            parent2 = selection_turnament(parents);

            // Create offspring through crossover
            Path child = crossover(parent1, parent2);

            // Apply mutation with given probability
            std::uniform_real_distribution<> mutDis(0.0, 1.0);
            if (mutDis(gen) < mutationRate) {
                if (mutDis(gen) < 0.5) {
                    child = mutate_swap(child);
                }
                else {
                    child = mutate_invert(child);
                }
            }

            offspring.push_back(child);

            // Update best solution if necessary
            if (child.getCost() < bestSolution.getCost()) {
                bestSolution = child;
            }
        }

        // Sort offspring by fitness (cost)
        std::sort(offspring.begin(), offspring.end(),
            [](const Path& a, const Path& b) {
                return a.getCost() < b.getCost();
            }
        );

        // Select μ best offspring as new parents (comma strategy)
        parents.clear();
        for (int i = 0; i < mu; i++) {
            parents.push_back(offspring[i]);
        }
    }

    // Stop timer
    timer.stop();
}


/**
 *
 * @param mu - parents (μ)
 * @param lambda  - offspring (λ)
 * @param generations (maximum) - maximum number of iterations 
 * @param mutationRate - mutation rate
 */

TSP_Result Genetic::run(int mu, int lambda, int generations, double mutationRate, std::optional<int> knownMinPathCost, std::optional<int> maxDurationMs) {
    TSP_Result result;


    if (knownMinPathCost.has_value()) {
        this->knownMinPathCost = knownMinPathCost.value();
    }

	if (maxDurationMs.has_value()) {
		this->maxDurationMs = maxDurationMs.value();
	}
	else {
		this->maxDurationMs = std::nullopt;
	}
	
    solve(mu, lambda, generations, mutationRate);
	
    result.bestPath = bestSolution.getNodesList();
	result.minPathCost = bestSolution.getCost();


	return result;
}