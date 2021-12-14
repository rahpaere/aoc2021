#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

bool is_easy(const string &display) {
	return (display.length() >= 2 && display.length() <= 4)
			|| display.length() == 7;
}

int main() {
	int easy = 0;
	while (cin) {
		cin.ignore(numeric_limits<streamsize>::max(), '|');
		for (int i = 0; i < 4; ++i) {
			string display;
			cin >> display;
			if (is_easy(display))
				++easy;
		}
	}
	cout << easy << endl;
	return EXIT_SUCCESS;
}
