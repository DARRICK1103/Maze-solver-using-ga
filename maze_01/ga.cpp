#include "ga.h"
#include "maze.h"
#include <algorithm>
#include <functional>
#include <iostream>

ga::ga(int population_size, int genome_size, float crossover_rate, float mutation_rate)
    : m_population_size(population_size), m_genome_size(genome_size), m_gene_size(2),
    m_crossover_rate(crossover_rate), m_mutation_rate(mutation_rate)
{
    for (int i = 0; i < population_size; i++)
    {
        // 50 * 2 = 100 chromosomes
        m_genomes.push_back(genome(genome_size * m_gene_size)); // store each chromosome in m_genome
    }
    m_generation = 1;
    
}

void ga::generation(const maze& maze)
{
    // compute fitness for all genomes
    for (auto& genome : m_genomes)
    {
        genome.fitness = maze.test_path(decode(genome));
    }
    

    // sort genomes by fitness
    std::sort(m_genomes.begin(), m_genomes.end(), std::greater<genome>()); 
}

void ga::performCrossoverAndMutation(const maze& maze)
{
    // initialize the children genomes vector
    std::vector<genome> children;
    children.reserve(m_genomes.size()); // all the children in the next generation

    // add the elite genomes to the children
    children.push_back(m_genomes[0]);
    children.push_back(m_genomes[1]);

    while (children.size() < m_genomes.size())
    {
        genome parent1 = select();
        genome parent2 = select();
        genome child = crossover(parent1, parent2);
        mutate(child);
        
        
        // survival selection

        if ((parent1.fitness < child.fitness) && (child.fitness >= parent2.fitness))
            children.push_back(child);
        else if ((parent2.fitness < child.fitness) && (child.fitness >= parent1.fitness))
            children.push_back(child);
        else if ((child.fitness >= parent1.fitness) && (child.fitness >= parent2.fitness))
            children.push_back(child);
    
    }
    m_generation++;
    m_genomes = std::move(children); // copy the children to m_genomes
}


float ga::get_current_generation_best()
{
    return m_genomes[0].fitness;
}

int ga::get_curent_generation()
{
    return m_generation;
}

float ga::get_total_fitness()
{
    float totalFitness = 0;
    
    for (const auto& genome : m_genomes)
    {
        
        totalFitness += genome.fitness;
    }
    return totalFitness;
}

void ga::printGeneration()
{
    std::cout << "\n\n\n------------------------------------------------------------------------------\n\n";
    std::cout << "\nGeneration " << m_generation << std::endl << std::endl;
    int count = 1;
    for (auto& genome : m_genomes)
    {
        std::cout << "\nChromosome "<< count << std::endl;
        for (int i = 0; i < genome.data.size(); i++)
        {
            std::cout << genome.data[i] << " ";
        }
        std::cout << "Fitness: " << genome.fitness << std::endl;
        std::cout << std::endl;
        count++;
    }
    std::cout << "\n\n---------------------------------------------------------------------------------\n\n";
}

const genome& ga::select()
{
    // only select the best third of the genomes and return the more fit
    const genome& genome1 = m_genomes[rand() % (m_genomes.size() / 2)];  
    const genome& genome2 = m_genomes[rand() % (m_genomes.size() / 2)];
    return genome1 > genome2 ? genome1 : genome2;
}

genome ga::crossover(const genome& parent1, const genome& parent2)
{
    genome child;
    float r = (float)(rand()) / (float)(RAND_MAX);
    if (r < m_crossover_rate)
    {
        /*
        std::cout << "\nBefore Crossover\n";
        std::cout << "Parent 1\n";
        for (int i = 0; i < parent1.data.size(); i++)
        {
            std::cout << parent1.data[i] << " ";
        }
        std::cout << "\nParent 2\n";
        for (int i = 0; i < parent2.data.size(); i++)
        {
            std::cout << parent2.data[i] << " ";
        }
        */

        // one point crossover for now
        int crossover_point = rand() % std::min(parent1.size(), parent2.size());

        child.data.insert(child.data.end(), parent1.data.begin(), parent1.data.begin() + crossover_point);
        child.data.insert(child.data.end(), parent2.data.begin() + crossover_point, parent2.data.end());
    }
    else
    {
        // TODO: find a best way to handle this case
        child = parent1;
        //std::cout << "\nCrossover does not perform !\n";
    }
    return child; // 
}

void ga::mutate(genome& genome)
{
    // 2 types of mutation - flip mutation and insertion mutation
    
    // flip mutation

    //std::cout << "\nBefore flip mutation\n";
    for (int i = 0; i < genome.size(); i++)
    {
        float r = (float)(rand()) / (float)(RAND_MAX);
        //std::cout << genome.data[i] << " ";
        if (r < m_mutation_rate)
        {
            genome.data[i] = (rand() % 2);
        }
    }
    /*
    std::cout << "\nAfter flip mutation\n";
    for (int i = 0; i < genome.size(); i++)
    {
        std::cout << genome.data[i] << " ";
    }
    */
    
    // insertion mutation
    float ir = (float)(rand()) / (float)(RAND_MAX);
    if (ir < m_mutation_rate)
    {
        int index = rand() % genome.size();
        //std::cout << "\nInsertion mutation bit: " << index << std::endl;
        genome.data.insert(genome.data.begin() + index, rand() % 2);
        genome.data.insert(genome.data.begin() + index, rand() % 2);
        /*
        for (int i = 0; i < genome.size(); i++)
        {
            std::cout << genome.data[i] << " ";
        }
        */
    }
    else
    {
        //std::cout << "\nInsertion mutation does not perform";
    }
    
    // TODO: deletion mutation
    // float dr = (float)(rand()) / (float)(RAND_MAX);
    // if (dr < m_mutation_rate)
    // {
    //   int index = rand() % genome.size() - 2;
    //   genome.data.erase(genome.data.begin() + index, genome.data.begin() + index + 1);
    // }
}

path ga::decode(const genome& genome) const
{
    path path;
    for (int i = 0; i < genome.size(); i += m_gene_size)
    {
        std::vector<position> directions = { position(-1, 0), position(0, 1), position(1, 0), position(0, -1) };
        int dir = genome[i] * 2 + genome[i + 1];
        path.directions.push_back({ directions[dir].x, directions[dir].y });
    }
    return path;
}

genome& ga::survivalSelection(const maze& maze, genome parent1, genome parent2, genome child)
{
    parent1.fitness = maze.test_path(decode(parent1));
    parent2.fitness = maze.test_path(decode(parent2));
    child.fitness = maze.test_path(decode(child));

    // Compare the fitness values and return the genome with the highest fitness
    if (parent1.fitness >= parent2.fitness && parent1.fitness >= child.fitness)
        return parent1;
    else if (parent2.fitness >= parent1.fitness && parent2.fitness >= child.fitness)
        return parent2;
    else
        return child;
}