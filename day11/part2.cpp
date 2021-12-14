#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

const int width = 10;

class SquidGrid {
public:
	void step();
	void charge(int row, int col);
	bool bright() const;

	vector<int> squids;
	vector<bool> flashing;
	unsigned flashes;
};

void SquidGrid::charge(int row, int col) {
	if (row < 0 || row >= width || col < 0 || col >= width
	    || flashing[row * width + col])
		return;

	++squids[row * width + col];
	if (squids[row * width + col] <= 9)
		return;

	++flashes;
	squids[row * width + col] = 0;
	flashing[row * width + col] = true;
	for (int i = row - 1; i <= row + 1; ++i)
		for (int j = col - 1; j <= col + 1; ++j)
			charge(i, j);
}

void SquidGrid::step() {
	fill(flashing.begin(), flashing.end(), false);
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < width; ++j)
			charge(i, j);
}

bool SquidGrid::bright() const {
	return all_of(flashing.begin(), flashing.end(), [](auto x) { return x; });
}

istream &operator>>(istream &in, SquidGrid &grid) {
	grid.flashes = 0;
	grid.squids.clear();
	transform(istream_iterator<char>(in), {}, back_inserter(grid.squids),
			[](char c) { return c - '0'; });
	grid.flashing.resize(grid.squids.size());
	return in;
}

ostream &operator<<(ostream &out, const SquidGrid &grid) {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < width; ++j) {
			if (grid.flashing[i * width + j])
				out << "\033[31m";
			out << grid.squids[i * width + j] << "\033[0m";
		}
		out << endl;
	}
	return out;
}

int main() {
	SquidGrid grid;
	cin >> grid;

	int steps = 0;
	while (!grid.bright()) {
		grid.step();
		++steps;
	}

	cout << steps << ":" << endl << grid;
	return EXIT_SUCCESS;
}
