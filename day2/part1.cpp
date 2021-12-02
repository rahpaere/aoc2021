#include <iostream>

using namespace std;

int main() {
	int horizontal = 0;
	int depth = 0;

	string direction;
	int distance;
	while (cin >> direction >> distance) {
		if (direction == "forward")
			horizontal += distance;
		else if (direction == "up")
			depth -= distance;
		else if (direction == "down")
			depth += distance;
		else
			cout << "unknown direction " << direction << endl;
	}

	cout << horizontal << " * " << depth << " = " << horizontal * depth << endl;
	return EXIT_SUCCESS;
}
