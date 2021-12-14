#include <iostream>

using namespace std;

const string bases = "BCFHKNOPSV."; // dot for end
const int num_bases = 11;

void read_polymer(uint64_t pairs[num_bases][num_bases]) {
	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			pairs[i][j] = 0;
	string s;
	cin >> s;
	s += '.'; // so last actual element has a next
	for (string::size_type i = 0; i + 1 < s.size(); ++i) {
		int here = bases.find(s[i]);
		int next = bases.find(s[i + 1]);
		++pairs[here][next];
	}
}

void read_rules(int rules[num_bases][num_bases]) {
	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			rules[i][j] = num_bases - 1; // end for no rule
	string arrow;
	char a, b, c;
	while (cin >> a >> b >> arrow >> c) {
		int here = bases.find(a);
		int next = bases.find(b);
		rules[here][next] = bases.find(c);
	}
}

void write_polymer(const uint64_t pairs[num_bases][num_bases]) {
	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			if (pairs[i][j])
				cout << bases[i] << bases[j] << ": "
				     << pairs[i][j] << endl;
	cout << endl;
}

void write_rules(const int rules[num_bases][num_bases]) {
	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			if (rules[i][j] != num_bases - 1)
				cout << bases[i] << bases[j] << " → "
				     << bases[rules[i][j]] << endl;
	cout << endl;
}

void insert(uint64_t pairs[num_bases][num_bases],
		const int rules[num_bases][num_bases]) {
	uint64_t insertions[num_bases][num_bases];
	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			insertions[i][j] = 0;

	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			if (rules[i][j] != num_bases - 1) {
				insertions[i][rules[i][j]] += pairs[i][j];
				insertions[rules[i][j]][j] += pairs[i][j];
				pairs[i][j] = 0;
			}

	for (int i = 0; i < num_bases; ++i)
		for (int j = 0; j < num_bases; ++j)
			pairs[i][j] += insertions[i][j];
}

void count(const uint64_t pairs[num_bases][num_bases],
		uint64_t counts[num_bases]) {
	for (int i = 0; i < num_bases; ++i) {
		counts[i] = 0;
		for (int j = 0; j < num_bases; ++j)
			counts[i] += pairs[i][j];
	}
}

uint64_t score(const uint64_t counts[num_bases]) {
	uint64_t most = counts[0];
	uint64_t least = counts[0];
	for (int i = 1; i < num_bases; ++i) {
		if (!counts[i])
			continue;
		if (counts[i] > most)
			most = counts[i];
		if (counts[i] < least)
			least = counts[i];
	}
	return most - least;
}

int main() {
	uint64_t pairs[num_bases][num_bases];
	read_polymer(pairs);

	int rules[num_bases][num_bases];
	read_rules(rules);

	for (int i = 1; i <= 40; ++i)
		insert(pairs, rules);

	uint64_t counts[num_bases];
	count(pairs, counts);
	cout << score(counts) << endl;

	return 0;
}
