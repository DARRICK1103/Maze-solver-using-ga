#pragma once

#include <vector>
#include "path.h"

class maze
{
public:
	maze();
	std::vector<std::vector<int>> getMaze();

	void printMaze();
	float test_path(const path& path) const;
	bool is_path(const position& pos) const;
	
	bool is_exit(const position& pos) const;
	
private:
	std::vector<std::vector<int>> m_data;
	int m_rows;
	int m_cols;
	int targetX;
	int targetY;
	position m_entrance;
	position m_exit;
};