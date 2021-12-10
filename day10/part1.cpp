#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

int points(const string &line) {
	stack<char> openings;
	for (char c: line) {
		if (c == '(' || c == '[' || c == '{' || c == '<') {
			openings.push(c);
			continue;
		}
		switch (c) {
		case ')':
			if (openings.empty() || openings.top() != '(')
				return 3;
			break;
		case ']':
			if (openings.empty() || openings.top() != '[')
				return 57;
			break;
		case '}':
			if (openings.empty() || openings.top() != '{')
				return 1197;
			break;
		case '>':
			if (openings.empty() || openings.top() != '<')
				return 25137;
			break;
		}
		openings.pop();
	}
	return 0;
}

int main() {
	int total = 0;
	string line;
	while (getline(cin, line))
		total += points(line);
	cout << total << endl;
	return EXIT_SUCCESS;
}
