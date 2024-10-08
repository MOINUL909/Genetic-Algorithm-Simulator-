#include <bits/stdc++.h>

const int CITY_COUNT = 5; // Number of cities
const int POPULATION_SIZE = 100; // Population size
const int GENERATIONS = 1000; // Number of generations
const double MUTATION_RATE = 0.01; // Mutation rate

// Function to calculate the distance between two cities
double distance(const std::pair<int, int>& city1, const std::pair<int, int>& city2) {
    return std::sqrt(std::pow(city2.first - city1.first, 2) + std::pow(city2.second - city1.second, 2));
}

// Class to represent the Traveling Salesman Problem
class TSP {
public:
    TSP(const std::vector<std::pair<int, int>>& cities) : cities(cities) {
        numCities = cities.size();
        bestDistance = std::numeric_limits<double>::max();
    }

    // Calculate the total distance of a given tour
    double calculateDistance(const std::vector<int>& tour) {
        double totalDistance = 0.0;
        for (size_t i = 0; i < tour.size(); ++i) {
            totalDistance += distance(cities[tour[i]], cities[tour[(i + 1) % tour.size()]]);
        }
        return totalDistance;
    }

    // Generate an initial population of tours
    void generateInitialPopulation() {
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            std::vector<int> tour(numCities);
            std::iota(tour.begin(), tour.end(), 0); // Fill with city indices
            std::shuffle(tour.begin(), tour.end(), std::mt19937{std::random_device{}()}); // Shuffle
            population.push_back(tour);
            fitness.push_back(calculateDistance(tour));
        }
    }

    // Perform selection using tournament selection
    void selection() {
        std::vector<std::vector<int>> newPopulation;
        std::vector<double> newFitness;

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            int index1 = rand() % POPULATION_SIZE;
            int index2 = rand() % POPULATION_SIZE;

            if (fitness[index1] < fitness[index2]) {
                newPopulation.push_back(population[index1]);
                newFitness.push_back(fitness[index1]);
            } else {
                newPopulation.push_back(population[index2]);
                newFitness.push_back(fitness[index2]);
            }
        }

        population = newPopulation;
        fitness = newFitness;
    }

    // Perform crossover between two parents
    std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
        std::vector<int> child(numCities, -1);
        int start = rand() % numCities;
        int end = rand() % numCities;

        if (start > end) std::swap(start, end);

        for (int i = start; i <= end; ++i) {
            child[i] = parent1[i];
        }

        int currentIndex = (end + 1) % numCities;
        for (int i = 0; i < numCities; ++i) {
            if (std::find(child.begin(), child.end(), parent2[i]) == child.end()) {
                child[currentIndex] = parent2[i];
                currentIndex = (currentIndex + 1) % numCities;
            }
        }

        return child;
    }

    // Perform mutation
    void mutate(std::vector<int>& tour) {
        for (size_t i = 0; i < tour.size(); ++i) {
            if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
                int swapIndex = rand() % numCities;
                std::swap(tour[i], tour[swapIndex]);
            }
        }
    }

    // Run the genetic algorithm
    void run() {
        generateInitialPopulation();

        for (int gen = 0; gen < GENERATIONS; ++gen) {
            selection();
            std::vector<std::vector<int>> newPopulation;

            // Crossover and mutation
            for (int i = 0; i < POPULATION_SIZE; i += 2) {
                std::vector<int> parent1 = population[rand() % POPULATION_SIZE];
                std::vector<int> parent2 = population[rand() % POPULATION_SIZE];
                std::vector<int> child1 = crossover(parent1, parent2);
                std::vector<int> child2 = crossover(parent2, parent1);
                mutate(child1);
                mutate(child2);
                newPopulation.push_back(child1);
                newPopulation.push_back(child2);
            }

            population = newPopulation;
            fitness.clear();

            for (const auto& tour : population) {
                fitness.push_back(calculateDistance(tour));
            }

            // Find the best tour
            auto minElement = std::min_element(fitness.begin(), fitness.end());
            if (minElement != fitness.end()) {
                double currentBestDistance = *minElement;
                if (currentBestDistance < bestDistance) {
                    bestDistance = currentBestDistance;
                    bestTour = population[std::distance(fitness.begin(), minElement)];
                }
            }

            // Visualization of the best tour found so far
            std::cout << "Generation " << gen + 1 << " | Best Distance: " << std::setprecision(6) << bestDistance << std::endl;
            printTour(bestTour);
        }
    }

    // Print the best tour
    void printTour(const std::vector<int>& tour) {
        std::cout << "Best Tour: ";
        for (int city : tour) {
            std::cout << city << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<std::pair<int, int>> cities;
    int numCities;
    std::vector<std::vector<int>> population;
    std::vector<double> fitness;
    double bestDistance;
    std::vector<int> bestTour;
};

// Function to check login credentials
bool login() {
    /*
UserId=Moinul
Password=Moinul909
*/
    std::string userId, password;
    std::cout << "Enter UserId: ";
    std::cin >> userId;
    std::cout << "Enter Password: ";
    std::cin >> password;

    // Check if the credentials match
    if (userId == "Moinul" && password == "Moinul909") {
        std::cout << "Login successful!\n";
        return true;
    } else {
        std::cout << "Invalid UserId or Password. Access denied!\n";
        return false;
    }
}

int main() {
    if (!login()) {
        return 0;
    }

    // Define cities with random coordinates (for simplicity)
    std::vector<std::pair<int, int>> cities = {
        {0, 0}, {1, 5}, {5, 2}, {7, 8}, {3, 6}
    };

    TSP tsp(cities);
    tsp.run();

    return 0;
}

