#include "ga.h"
#include "maze.h"
#include <iostream>
#include <fstream>
using namespace std;

const float crossover_rate = 0.7;
const float mutation_rate = 0.05;
const int population_size = 100;
const int genome_size = 50;
const int num_generations = 50;

// output file
ofstream averageFitnessFile;
ofstream bestFitnessFile;
ofstream bestChromosmeFile;

int main()
{
    srand(time(nullptr));

    // Create an instance of the maze class
    maze myMaze;
    averageFitnessFile.open("AverageFitness.txt");
    bestChromosmeFile.open("BestChromosome.txt");
    bestFitnessFile.open("BestFitness.txt");
    // Create an instance of the ga class
    ga myGA(population_size, genome_size, crossover_rate, mutation_rate); // initialise chromosome 
    myGA.generation(myMaze);
    myGA.printGeneration();

    float bestFitness = 0;
    float averageFitness = 0;

    for (int i = 0; i < num_generations; i++)
    {
        myGA.generation(myMaze); // calculate fitness
        myGA.printGeneration();
        myGA.performCrossoverAndMutation(); // perform crossover and mutation 
        averageFitness = myGA.get_total_fitness()/population_size;
        if (bestFitness < myGA.get_current_generation_best())
        {
            bestFitness = myGA.get_current_generation_best();
        }
        averageFitnessFile << averageFitness << endl;
        bestFitnessFile << bestFitness << endl;
    }

    averageFitnessFile.close();
    bestChromosmeFile.close();
    bestFitnessFile.close();
    

    return 0;
}