#include <iostream>
#include <vector>

const int SIZE = 80;

class Maze {
private:
	bool visited[SIZE * SIZE];
	bool walls[SIZE * SIZE][2] = { 0 };

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
	int nextCell(int pos, int n) {
		int optionCount = 0;
		int options[4];

		// up
		if (pos >= n && !visited[pos - n]) {
			options[optionCount++] = -n;
		}
		// right
		if ((pos + 1) % n != 0 && !visited[pos + 1]) {
			options[optionCount++] = 1;
		}
		// down
		if (pos < n * (n - 1) && !visited[pos + n]) {
			options[optionCount++] = n;
		}
		// left
		if (pos % n != 0 && !visited[pos - 1]) {
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
			int delta = nextCell(pos, SIZE);
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
		int pos = 0;

		for (int i = 0; i < SIZE * SIZE; i++) {
			visited[i] = false;
		}

		for (int i = 0; i < SIZE * SIZE; i++) {
			walls[i][0] = walls[i][1] = false;
		}

		std::srand(std::time(nullptr));

		randomDfs(pos);
	}

	bool isVisited(int index) const { return visited[index]; }
	bool rightOpen(int index) const { return walls[index][0]; }
	bool downOpen(int index) const { return walls[index][1]; }

	bool rightOpen(int x, int y) const { return walls[index(x, y)][0]; }
	bool downOpen(int x, int y) const { return walls[index(x, y)][1]; }

};