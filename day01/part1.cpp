#include <iostream>

using namespace std;

int main() {
	int prev;
	cin >> prev;

	int cur;
	int count = 0;
	while (cin >> cur) {
		if (cur > prev)
			++count;
		prev = cur;
	}

	cout << count << endl;

	return EXIT_SUCCESS;
}
