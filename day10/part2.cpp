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

uint64_t points(const string &line) {
	stack<char> openings;
	for (char c: line) {
		if (c == '(' || c == '[' || c == '{' || c == '<') {
			openings.push(c);
			continue;
		}
		switch (c) {
		case ')':
			if (openings.empty() || openings.top() != '(')
				return 0;
			break;
		case ']':
			if (openings.empty() || openings.top() != '[')
				return 0;
			break;
		case '}':
			if (openings.empty() || openings.top() != '{')
				return 0;
			break;
		case '>':
			if (openings.empty() || openings.top() != '<')
				return 0;
			break;
		}
		openings.pop();
	}
	
	uint64_t total = 0;
	while (!openings.empty()) {
		total *= 5;
		switch (openings.top()) {
		case '(': total += 1; break;
		case '[': total += 2; break;
		case '{': total += 3; break;
		case '<': total += 4; break;
		}
		openings.pop();
	}
	return total;
}

int main() {
	vector<string> lines;
	vector<uint64_t> scores;
	copy(istream_iterator<string>(cin), {}, back_inserter(lines));
	transform(lines.begin(), lines.end(), back_inserter(scores), points);
	auto e = remove(scores.begin(), scores.end(), 0);
	auto m = scores.begin() + (e - scores.begin()) / 2;
	nth_element(scores.begin(), m, e);
	cout << *m << endl;
	return EXIT_SUCCESS;
}
