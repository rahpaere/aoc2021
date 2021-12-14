#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

/*

a: 0 23 56789  8
b: 0   456 89  6
c: 01234  789  8
d:   23456 89  7
e: 0 2   6 8   4
f: 01 3456789  9
g: 0 23 56 89  7

   6255456376

# segments to digit
0:
1:
2: 1
3: 7
4: 4
5: 2 3 5
6: 0 6 9
7: 8

distinguish 2 3 5: has b then 5, else has f then 3, else 2
distinguish 0 6 9: lacks d then 0, else has c then 9, else 6

# appearances to segment
4: e
6: b
7: d g
8: a c
9: f

distinguish d g: d is in 4, g isn't
distinguish a c: c is in 4, a isn't

*/

struct Evidence {
	unsigned counts[7];
	string four;
};

// Record one of the ten unique digit displays in the log of evidence.
void record(string display, Evidence &e) {
	for (char c: display)
		++e.counts[c - 'a'];
	if (display.length() == 4)
		e.four = display;
}

// Based on the evidence, is this segment used to display four?
bool in_four(const Evidence &e, char segment) {
	return e.four.find(segment) != string::npos;
}

// Based on the evidence, deduce the mixed-up wiring.
// The mapping is indexed by desired segment number (0-7 for a-g)
// yielding character ('a'-'g') on broken display
void deduce_mapping(const Evidence &e, char mapping[7]) {
	for (char i = 'a'; i <= 'g'; ++i)
		switch (e.counts[i - 'a']) {
		case 4: mapping['e' - 'a'] = i; break;
		case 6: mapping['b' - 'a'] = i; break;
		case 7: mapping[(in_four(e, i) ? 'd' : 'g') - 'a'] = i; break;
		case 8: mapping[(in_four(e, i) ? 'c' : 'a') - 'a'] = i; break;
		case 9: mapping['f' - 'a'] = i; break;
		}
}

// Get the intended digit from a mixed-up display.
unsigned read_digit(const string &display, const char mapping[7]) {
	switch (display.length()) {
	case 2: return 1;
	case 3: return 7;
	case 4: return 4;
	case 5: if (display.find(mapping['b' - 'a']) != string::npos)
			return 5;
		else if (display.find(mapping['f' - 'a']) != string::npos)
			return 3;
		else
			return 2;
	case 6: if (display.find(mapping['d' - 'a']) == string::npos)
			return 0;
		else if (display.find(mapping['c' - 'a']) != string::npos)
			return 9;
		else
			return 6;
	case 7: return 8;
	}
	// cannot happen on valid input
	return numeric_limits<unsigned>::max();
}

// Get the intended multi-digit number from a sequence of mixed-up displays.
unsigned read_display(istream &in, const char mapping[7]) {
	unsigned n = 0;
	string digit;
	while (in >> digit)
		n = (n * 10) + read_digit(digit, mapping);
	return n;
}

// Gather evidence over time from a multi-digit display
// and then read a number from it.
unsigned deduce_and_read_display(istream &in) {
	string line;
	if (!getline(in, line) || line.empty())
		return numeric_limits<unsigned>::max();

	istringstream iss { line };

	Evidence e {};
	for (int i = 0; i < 10; ++i) {
		string display;
		iss >> display;
		record(display, e);
	}

	iss.ignore(numeric_limits<streamsize>::max(), '|');

	char mapping[7] {};
	deduce_mapping(e, mapping);
	return read_display(iss, mapping);
}

int main() {
	unsigned total = 0;
	for (;;) {
		unsigned display = deduce_and_read_display(cin);
		if (!cin)
			break;
		total += display;
	}
	cout << "Total: " << total << endl;
	return EXIT_SUCCESS;
}
