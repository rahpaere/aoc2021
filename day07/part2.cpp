#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

int cost(vector<int> &positions, int target) {
	int total = 0;
	for (int i: positions) {
		int d = abs(target - i);
		total += d * (d + 1) / 2;
	}
	return total;
}

int main() {
	vector<int> positions;
	copy(istream_iterator<int>(cin), {}, back_inserter(positions));

	int limit = *max_element(positions.begin(), positions.end());
	int best = numeric_limits<int>::max();
	for (int i = 0; i < limit; ++i)
		best = min(best, cost(positions, i));

	cout << best << endl;
	return EXIT_SUCCESS;
}
