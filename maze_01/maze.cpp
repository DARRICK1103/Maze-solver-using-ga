#include "maze.h"
#include <iostream>
#include <cmath>
#include <unordered_set>
using namespace std;

maze::maze()
{
	m_data = {
        { 1, 1, 1, 1, 1, 0, 0, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0, 1, 0, 1 },
        { 0, 0, 1, 0, 1, 1, 1, 0, 0, 1 },
        { 1, 0, 1, 1, 1, 0, 1, 1, 0, 1 },
        { 0, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
        { 1, 0, 1, 1, 1, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 0, 0, 1, 0, 1 },
        { 0, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
        { 1, 1, 1, 1, 1, 0, 0, 1, 1, 1 },
        { 0, 0, 1, 0, 0, 1, 1, 0, 0, 1 },
    };
    m_entrance.x = 0;
    m_entrance.y = 0;
    m_cols = m_data[0].size(); // width of the maze
    m_rows = m_data.size();

    targetX = 9;
    targetY = 9;

    m_exit.x = 9;
    m_exit.y = 9;
}

vector<vector<int>> maze::getMaze()
{
    return m_data;
}

void maze::printMaze()
{
   

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            if (i == 0 && j == 0) {
                std::cout << "S ";
            }
            else if (i == m_rows - 1 && j == m_cols - 1) {
                std::cout << "T ";
            }
            else if (m_data[i][j] == 1) {
                std::cout << ". ";
            }
            else {
                std::cout << "X ";
            }
        }
        std::cout << endl << endl;
    }
}


// this method takes a path as input and returns a fitness value for that path.
// the method goes through each direction in the path, checks if the direction leads to a wall or has been visited before, and increments a penalty in such cases.
// the fitness value is calculated as the inverse of the distance from the last position to the exit, plus the penalty.
float maze::test_path(const path& path) const
{
    float penalty = 0;
    position current; // starting point
    std::unordered_set<int> visited;

    // TODO: add to global parameters
    float visited_weight = 0.1;
    float wall_weight = 0.1;
    float distance_weight = 1;

    bool findTarget = false;

    for (const auto& direction : path.directions)
    {

        position next = current + direction;
        if (is_exit(next))
        {
            current = next;
            findTarget = true;
            break;
        }
        else
        {
            if (is_path(next))
            {
                if (visited.find(next.x * m_cols + next.y) != visited.end()) // if visited the point before
                    penalty += visited_weight;
                else
                    visited.insert(next.x * m_cols + next.y); // store the visited node
                current = next;
            }
            else
            {
                penalty += wall_weight;
            }
        }
    }
    float distance = distance_weight * std::sqrt(std::pow(current.x - m_exit.x, 2) + std::pow(current.y - m_exit.y, 2));
    return 1.0f / (distance + penalty + 1);
}

bool maze::is_path(const position& pos) const
{
    if (pos.x >= 0 && pos.x < m_cols && pos.y >= 0 && pos.y < m_rows)
    {
        if (m_data[pos.y][pos.x] == 1) // 1 means path
        {
            return true;
        }
        else
        {
            return false; // obstacles
        }
    }
    else
    {
        return false;
        // Handle the case when the position is outside the valid range
        // Return false or throw an exception, depending on your desired behavior
    }

}

bool maze::is_exit(const position& pos) const
{
    if (pos.x >= 0 && pos.x < m_cols && pos.y >= 0 && pos.y < m_rows)
    {
        if (pos.x == targetX && pos.y == targetY) // target found
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
        // Handle the case when the position is outside the valid range
        // Return false or throw an exception, depending on your desired behavior
    }

}
