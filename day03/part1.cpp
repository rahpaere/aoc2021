#include <iostream>

using namespace std;

const int width = 12;

int main() {
	int counts[width] {};
	int total {};

	string line;
	while (cin >> line) {
		for (int i = 0; i < width; ++i)
			if (line[i] == '1')
				++counts[i];
		++total;
	}

	int gamma {};
	for (int i = 0; i < width; ++i) {
		if (counts[i] * 2 >= total)
			gamma |= 1 << (width - 1 - i);
	}

	int epsilon = gamma ^ ((1 << width) - 1);

	cout << gamma << " * " << epsilon << " = " << gamma * epsilon << endl;

	return EXIT_SUCCESS;
}
