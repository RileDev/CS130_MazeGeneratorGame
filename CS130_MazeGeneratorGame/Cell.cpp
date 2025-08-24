#include <vector>

class Cell {
private:
	float row;
	float col;
	bool walls[4] = {true, true, true, true};
	bool visited = false;
	std::vector<Cell> neighbors;

public:
	Cell(float x, float y) {
		this->row = x;
		this->col = y;
	}

	float getRow() {
		return this->row;
	}

	float getColumn() {
		return this->col;
	}

};