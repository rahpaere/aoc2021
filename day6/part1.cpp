#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

void step(int lanternfish[9]) {
	int spawning = lanternfish[0];
	for (int i = 0; i < 8; ++i)
		lanternfish[i] = lanternfish[i + 1];
	lanternfish[8] = spawning;
	lanternfish[6] += spawning;
}

int main() {
	int lanternfish[9] {};

	int age;
	while (cin >> age)
		++lanternfish[age];

	for (int i = 0; i < 80; ++i)
		step(lanternfish);

	cout << accumulate(lanternfish, lanternfish + 9, 0) << endl;
	return EXIT_SUCCESS;
}
