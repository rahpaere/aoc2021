#include <algorithm>
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <variant>
#include <vector>

using namespace std;

using Range = pair<int, int>;
using Coord = tuple<int, int, int>;
using Cuboid = tuple<Range, Range, Range>;

istream &operator>>(istream &in, Range &r) {
	char dump;
	in >> dump >> dump >> get<0>(r) >> dump >> dump >> get<1>(r);
	get<0>(r) = max(get<0>(r), -50);
	get<1>(r) = min(get<1>(r), 50);
	return in;
}

istream &operator>>(istream &in, Cuboid &c) {
	char comma;
	return in >> get<0>(c) >> comma >> get<1>(c) >> comma >> get<2>(c);
}

void initialize(uint8_t reactor[100][100][100], bool value, const Cuboid &c) {
	for (int x = get<0>(c).first; x <= get<0>(c).second; ++x)
		for (int y = get<1>(c).first; y <= get<1>(c).second; ++y)
			for (int z = get<2>(c).first; z <= get<2>(c).second; ++z)
				reactor[x + 50][y + 50][z + 50] = value;
}

int count_on(uint8_t reactor[100][100][100]) {
	int on = 0;
	for (int x = 0; x < 100; ++x)
		for (int y = 0; y < 100; ++y)
			for (int z = 0; z < 100; ++z)
				on += reactor[x][y][z];
	return on;
}

int main() {
	uint8_t reactor[100][100][100] {};
	string op;
	Cuboid c;
	while (cin >> op >> c)
		initialize(reactor, op == "on", c);
	cout << count_on(reactor) << endl;
	return EXIT_SUCCESS;
}
