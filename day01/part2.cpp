#include <iostream>

using namespace std;

const int n = 3;

int main() {
	int window[n + 1];
	int i = 0;
	while (i < n) {
		cin >> window[i];
		++i;
	}

	int count = 0;
	while (cin >> window[i]) {
		int oldest = (i + 1) % (n + 1);
		if (window[i] > window[oldest])
			++count;
		i = oldest;
	}

	cout << count << endl;

	return EXIT_SUCCESS;
}
