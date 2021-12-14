#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

using Rules = map<pair<char, char>, char>;
using Histogram = map<char, uint64_t>;

struct Polymer {
	void insert(const Rules &rules);
	void insert(const Rules &rules, unsigned iterations);
	Histogram count() const;
	uint64_t score() const;

	map<pair<char, char>, uint64_t> pairs;
};

istream &operator>>(istream &in, Polymer &polymer) {
	polymer.pairs.clear();

	string s;
	in >> s;
	if (s.empty())
		return in;

	auto it = s.begin();
	while (it + 1 != s.end()) {
		++polymer.pairs[{ *it, *(it + 1) }];
		++it;
	}
	++polymer.pairs[{ *it, '\0' }];

	return in;
}

istream &operator>>(istream &in, Rules &rules) {
	string arrow;
	char left, right, insert;
	while (in >> left >> right >> arrow >> insert)
		rules[{ left, right }] = insert;
	return in;
}

void Polymer::insert(const Rules &rules) {
	map<pair<char, char>, uint64_t> insertions;

	auto it = pairs.begin();
	while (it != pairs.end()) {
		auto rule = rules.find(it->first);
		if (rule == rules.end()) {
			insertions.insert(*it);
			++it;
			continue;
		}

		insertions[{ it->first.first, rule->second }] += it->second;
		insertions[{ rule->second, it->first.second }] += it->second;
		it = pairs.erase(it);
	}

	pairs = move(insertions);
}

void Polymer::insert(const Rules &rules, unsigned iterations) {
	for (unsigned i = 0; i < iterations; ++i)
		insert(rules);
}

Histogram Polymer::count() const {
	Histogram h;
	for (const auto &[key, count]: pairs)
		h[key.first] += count;
	return h;
}

uint64_t Polymer::score() const {
	uint64_t most = numeric_limits<uint64_t>::min();
	uint64_t least = numeric_limits<uint64_t>::max();
	for (const auto &[base, count]: count()) {
		most = max(most, count);
		least = min(least, count);
	}
	return most - least;
}

int main() {
	Polymer polymer;
	Rules rules;
	cin >> polymer >> rules;
	polymer.insert(rules, 40);
	cout << polymer.score() << endl;
	return EXIT_SUCCESS;
}
