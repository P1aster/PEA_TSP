#include "SA.h"
#include "algorithms/RNN.h"


/*
	Constructor for the Simulated Annealing(SA) algorithm.
	Initializes the graph, cooling schema, number of nodes, and adjacency matrix.
*/
SA::SA(Graph graph, CoolingSchema schema) {
	this->graph = graph;
	this->minPathCost = INT_MAX;
	this->initialTemperature = 10000;
	this->finalTemperature = 0.0001;
	this->coolingRate = 0.995;
	this->coolingSchema = schema;
	this->nodesNumber = graph.getNodesNumber();
	this->matrix = graph.getMatrix();

}

TSP_Result SA::run(double initialTemperature, double finalTemperature, double coolingRate) {
	
	this->initialTemperature = initialTemperature; // Starting temperature for the algorithm.
	this->finalTemperature = finalTemperature;     // Temperature at which the algorithm terminates.
	this->coolingRate = coolingRate;               // Rate at which the temperature decreases.

	TSP_Result result;

	simulatedAnnealing();

	result.bestPath = bestPath;         // Best path found during the execution.
	result.minPathCost = minPathCost;   // Total cost of the best path.
	return result;

}

/*
	Core function implementing the Simulated Annealing algorithm.
	Iteratively explores the solution space to find a near-optimal solution to the TSP.
*/
void SA::simulatedAnnealing() {
	int currentPathCost = 0;
	std::vector<int> currentPath;

	double temperature = initialTemperature;

	// Generate an initial solution using the Repeated Nearest Neighbour heuristic.
	RNN rnn(graph);
	TSP_Result rnn_result = rnn.findRepeatedNearestNaighbour();

	currentPath = rnn_result.bestPath;
	bestPath = currentPath;
	currentPathCost = rnn_result.minPathCost;
	minPathCost = currentPathCost;
	
	// Main loop: continues until the system has cooled down to the final temperature.
	while (temperature > finalTemperature) {
		
		// Generate a new solution by swapping two random nodes in the current path.
		std::vector<int> newPath = determinNewSolution(currentPath);
		int newPathCost = calculateCost(newPath);

		// Decide whether to accept the new solution based on the acceptance probability.
		if (accept(currentPathCost, newPathCost, temperature)) {
			currentPath = newPath;
			currentPathCost = newPathCost;

			// Update the best solution found if the new path is better (has lower cost).
			if (currentPathCost < minPathCost) {
				bestPath = currentPath;
				minPathCost = currentPathCost;
			}
		}

		
		std::cout << " " << temperature << " " << currentPathCost << " " << minPathCost << std::endl;
		// Cool down the temperature according to the selected cooling schema.
		switch (coolingSchema) {
		case CoolingSchema::Linear:
			temperature = std::max(temperature - coolingRate, finalTemperature);
			break;
		case CoolingSchema::Exponential:
			temperature *= coolingRate;
			break;
		case CoolingSchema::Logarithmic:
			temperature /= (1 + coolingRate * log(1 + temperature));
			break;
		}

	}
}

/*
	Calculates the total cost of a given path.
	Sums the distances between consecutive nodes and returns to the starting node.
*/
int SA::calculateCost(std::vector<int>& curr) {
	int cost = 0;
	for (int i = 0; i < nodesNumber - 1; ++i) {
		cost += matrix[curr[i]][curr[i + 1]];
	}

	// Add the distance from the last node back to the first one, to complete the circle.
	cost += matrix[curr[nodesNumber - 1]][curr[0]];
	return cost;
}


// Generates a new candidate solution by performing a random swap of two nodes in the current path.
std::vector<int> SA::determinNewSolution(const std::vector<int>& currPath) {
	std::uniform_int_distribution<> dis(0, nodesNumber - 1);
	std::vector<int> newPath = currPath;

	// Randomly select two different nodes in the path.
	int i = dis(gen);
	int j = dis(gen);
	while (i == j) {
		j = dis(gen);
	}
	// Swap the positions of the two selected nodes to create a new path.
	std::swap(newPath[i], newPath[j]);

	return newPath;
}


/*
	Determines whether to accept the new solution based on the Metropolis criterion.
	Accepts better solutions or probabilistically accepts worse solutions based on temperature.
*/
bool SA::accept(int currCost, int newCost, double temperature) {
	if (newCost < currCost) {
		return true; // Always accept if the new solution is better (lower cost).
	} else {
		// Calculate acceptance probability for worse solutions.
		std::uniform_real_distribution<> prob_dist(0.0, 1.0);
		double probability = std::exp((currCost - newCost) / temperature);
		// Accept the worse solution with a probability that decreases as temperature decreases.
		return prob_dist(gen) < probability;
	}
}
