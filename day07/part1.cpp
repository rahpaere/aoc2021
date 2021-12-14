#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

int main() {
	vector<int> positions;
	copy(istream_iterator<int>(cin), {}, back_inserter(positions));

	auto m = positions.begin() + positions.size() / 2;
	nth_element(positions.begin(), m, positions.end());
	int target = *m;

	vector<int> costs(positions.size());
	transform(positions.begin(), positions.end(), back_inserter(costs),
		  [target](auto x) { return abs(x - target); });

	cout << accumulate(costs.begin(), costs.end(), 0) << endl;
	return EXIT_SUCCESS;
}
