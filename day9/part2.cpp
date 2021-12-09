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

int mark_basin(vector<vector<char>> &heights, size_t i, size_t j, char mark) {
	if (i < 0 || i >= heights.size() || j < 0 || j >= heights[i].size())
		return 0;
	if (heights[i][j] < '0' || heights[i][j] >= '9')
		return 0;
	heights[i][j] = mark;
	return 1 + mark_basin(heights, i + 1, j, mark)
		 + mark_basin(heights, i - 1, j, mark)
		 + mark_basin(heights, i, j + 1, mark)
		 + mark_basin(heights, i, j - 1, mark);
}

int main() {
	vector<vector<char>> heights;
	string line;
	while (getline(cin, line))
		heights.emplace_back(line.begin(), line.end());

	vector<int> basins;
	for (size_t i = 0; i < heights.size(); ++i)
		for (size_t j = 0; j < heights[i].size(); ++j) {
			int n = mark_basin(heights, i, j, 'a' + (basins.size() % 26));
			if (n)
				basins.push_back(n);
		}

	for (const auto &row: heights) {
		copy(row.begin(), row.end(), ostream_iterator<char>(cout));
		cout << endl;
	}
	cout << endl;

	auto nth = basins.begin() + 3;

	partial_sort(basins.begin(), nth, basins.end(), greater<int>());
	copy(basins.begin(), nth, ostream_iterator<int>(cout, " * "));
	cout << " = "
	     << accumulate(basins.begin(), nth, 1, multiplies<int>())
	     << endl;
	return EXIT_SUCCESS;
}
