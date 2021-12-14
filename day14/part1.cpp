#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

struct Rule {
	char left;
	char right;
	char insert;
};

istream &operator>>(istream &in, Rule &rule) {
	string arrow;
	return in >> rule.left >> rule.right >> arrow >> rule.insert;
}

bool apply_rule(list<char> &polymer, list<char>::iterator &left,
		list<char>::iterator right, const Rule &rule) {
	if (*left == rule.left && *right == rule.right) {
		polymer.insert(right, rule.insert);
		return true;
	} else {
		return false;
	}
}

void insertions(list<char> &polymer, const vector<Rule> &rules) {
	auto right = polymer.begin();
	auto left = right;
	while (++right != polymer.end()) {
		for (const Rule &rule: rules) {
			if (apply_rule(polymer, left, right, rule))
				break;
		}
		left = right;
	}
}

ostream &operator<<(ostream &out, const list<char> &polymer) {
	copy(polymer.begin(), polymer.end(), ostream_iterator<char>(out));
	return out << endl;
}

int score(const list<char> &polymer) {
	vector<char> elements(polymer.begin(), polymer.end());
	sort(elements.begin(), elements.end());

	int most = numeric_limits<int>::min();
	int least = numeric_limits<int>::max();
	auto i = elements.begin();
	auto j = i + 1;
	for (;;) {
		if (j == elements.end() || *i != *j) {
			int distance = static_cast<int>(j - i);
			most = max(distance, most);
			least = min(distance, least);
			if (j == elements.end())
				break;
			i = j;
		}
		++j;
	}

	return most - least;
}

int main() {
	string polymerstr;
	cin >> polymerstr;
	list<char> polymer(polymerstr.begin(), polymerstr.end());

	vector<Rule> rules;
	copy(istream_iterator<Rule>(cin), {}, back_inserter(rules));

	for (int i = 1; i <= 10; ++i)
		insertions(polymer, rules);
	cout << score(polymer) << endl;
	return EXIT_SUCCESS;
}
