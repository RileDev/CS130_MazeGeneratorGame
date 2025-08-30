#include <iostream>
#include <vector>

const int SIZE = 20;

class Maze {
private:
	int startPos = 0;
	bool visited[SIZE * SIZE];
	bool walls[SIZE * SIZE][2] = { 0 }; //cuva se desno i dole 

	int index(int x, int y) const {
		return y * SIZE + x;
	}

	int xOf(int i) const {
		return i % SIZE;
	}

	int yOf(int i) const {
		return i / SIZE;
	}


protected:
	int nextCell(int pos) {
		int optionCount = 0;
		int options[4];

		// gornja celija
		if (pos >= SIZE && !visited[pos - SIZE]) {
			options[optionCount++] = -SIZE;
		}
		// desna celija
		if ((pos + 1) % SIZE != 0 && !visited[pos + 1]) {
			options[optionCount++] = 1;
		}
		// donja celija
		if (pos < SIZE * (SIZE - 1) && !visited[pos + SIZE]) {
			options[optionCount++] = SIZE;
		}
		// leva celija
		if (pos % SIZE != 0 && !visited[pos - 1]) {
			options[optionCount++] = -1;
		}

		if (optionCount == 0)
			return 0;

		return options[std::rand() % optionCount];
	}

	void connect(int pos1, int pos2) {
		if (pos2 == pos1 + 1) {
			//pos2 je desno od pos1
			walls[pos1][0] = true;
		}
		else if (pos2 == pos1 - 1) {
			//pos2 je levo od pos1
			walls[pos2][0] = true;
		}
		else if (pos2 == pos1 + SIZE) {
			//pos2 je ispod pos1
			walls[pos1][1] = true;
		}
		else if (pos2 == pos1 - SIZE) {
			//pos2 je iznad pos1
			walls[pos2][1] = true;
		}
	}

	void randomDfs(int start) {
		if (start < 0 || start >= SIZE * SIZE) return;
		std::vector<int> stack;
		stack.reserve(SIZE * SIZE);

		visited[start] = true;
		stack.push_back(start);

		while (!stack.empty()) {
			int pos = stack.back();
			int delta = nextCell(pos);
			if (delta == 0) {
				stack.pop_back();
				continue;
			}
			int next = pos + delta;
			connect(pos, next);
			visited[next] = true;
			stack.push_back(next);
		}
	}

public:
	void createMazeDfs() {

		for (int i = 0; i < SIZE * SIZE; i++) {
			visited[i] = false;
		}

		for (int i = 0; i < SIZE * SIZE; i++) {
			walls[i][0] = walls[i][1] = false;
		}

		std::srand(std::time(nullptr));

		randomDfs(startPos);
	}

	bool isVisited(int index) const { 
		return visited[index]; 
	}
	bool rightOpen(int index) const { 
		return walls[index][0]; 
	}
	bool downOpen(int index) const { 
		return walls[index][1]; 
	}

	bool rightOpen(int x, int y) const { 
		return walls[index(x, y)][0]; 
	}
	bool downOpen(int x, int y) const { 
		return walls[index(x, y)][1]; 
	}

	int getStartPos() {
		return this->startPos;
	}

	void setStartPos(int start) {
		if (start < 0 || start >= SIZE * SIZE) {
			this->startPos = 0;
		}
		else {
			this->startPos = start;
		}
	}

};