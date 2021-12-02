#include <iostream>

using namespace std;

int main() {
	int horizontal = 0;
	int depth = 0;
	int aim = 0;

	string direction;
	int distance;
	while (cin >> direction >> distance) {
		if (direction == "forward") {
			horizontal += distance;
			depth += aim * distance;
		} else if (direction == "up") {
			aim -= distance;
		} else if (direction == "down") {
			aim += distance;
		} else {
			cout << "unknown direction " << direction << endl;
		}
	}

	cout << horizontal << " * " << depth << " = " << horizontal * depth << endl;
	return EXIT_SUCCESS;
}
