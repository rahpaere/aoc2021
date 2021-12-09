#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

bool lowpoint(const vector<vector<char>> &heights, size_t i, size_t j) {
	if (i > 0 && heights[i - 1][j] <= heights[i][j])
		return false;
	if (i + 1 < heights.size() && heights[i + 1][j] <= heights[i][j])
		return false;
	if (j > 0 && heights[i][j - 1] <= heights[i][j])
		return false;
	if (j + 1 < heights[i].size() && heights[i][j + 1] <= heights[i][j])
		return false;
	return true;
}

int main() {
	vector<vector<char>> heights;
	string line;
	while (getline(cin, line))
		heights.emplace_back(line.begin(), line.end());

	int sum = 0;
	for (size_t i = 0; i < heights.size(); ++i)
		for (size_t j = 0; j < heights[i].size(); ++j)
			if (lowpoint(heights, i, j))
				sum += heights[i][j] - '0' + 1;
	
	cout << sum << endl;
	return EXIT_SUCCESS;
}
