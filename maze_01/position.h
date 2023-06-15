#pragma once

struct position {
	int x;
	int y;

	position(int x = 0, int y = 0) : x(x), y(y) {}

	int getPosition(int x, int y)
	{
		return x + y * 10; // 10 = width of the maze
	}

	position operator+(const position& other) const {
		return position(x + other.x, y + other.y);
	}
};