#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

const int width = 100;
const int REP = 5;

const uint16_t infinity = numeric_limits<uint16_t>::max();

void read_grid(uint8_t *grid) {
	for (int i = 0; i < width * width; ++i) {
		char c;
		cin >> c;
		grid[i] = c - '0';
	}
}

uint8_t get_risk(uint8_t *grid, int i, int j) {
	uint16_t delta = i / width + j / width;
	i %= width;
	j %= width;
	return (grid[i * width + j] - 1 + delta) % 9 + 1;
}

void consider(uint8_t *grid, uint16_t *cost, int si, int sj, int di, int dj) {
	if (si < 0 || si >= REP * width || sj < 0 || sj >= REP * width)
		return;
	uint16_t c = cost[si * REP * width + sj];
	if (c == infinity)
		return;
	c += get_risk(grid, di, dj);
	cost[di * REP * width + dj] = min(cost[di * REP * width + dj], c);
}

void explore(uint8_t *grid, uint16_t *cost) {
	for (int w = 2; w <= REP * width; ++w) {
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				consider(grid, cost, i - 1, j, i, j);
				consider(grid, cost, i + 1, j, i, j);
				consider(grid, cost, i, j - 1, i, j);
				consider(grid, cost, i, j + 1, i, j);
			}
		}
	}
}

int main() {
	uint8_t *grid = new uint8_t[width * width];
	read_grid(grid);

	uint16_t *cost = new uint16_t[width * REP * width * REP];
	cost[0] = 0;
	for (int i = 1; i < width * REP * width * REP; ++i)
		cost[i] = infinity;

	explore(grid, cost);
	cout << cost[width * REP * width * REP - 1] << endl;
	return 0;
}
