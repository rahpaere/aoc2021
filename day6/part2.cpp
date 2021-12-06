#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

void step(unsigned long long lanternfish[9]) {
	unsigned long long spawning = lanternfish[0];
	for (unsigned long long i = 0; i < 8; ++i)
		lanternfish[i] = lanternfish[i + 1];
	lanternfish[8] = spawning;
	lanternfish[6] += spawning;
}

int main() {
	unsigned long long lanternfish[9] {};

	unsigned long long age;
	while (cin >> age)
		++lanternfish[age];

	for (unsigned long long i = 0; i < 256; ++i)
		step(lanternfish);

	cout << accumulate(lanternfish, lanternfish + 9, 0ULL) << endl;
	return EXIT_SUCCESS;
}
