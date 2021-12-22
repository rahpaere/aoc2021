#include <algorithm>
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <variant>
#include <vector>

using namespace std;

const int spaces = 10;
const int points = 21;

class Superposition {
public:
	friend ostream &operator<<(ostream &, const Superposition &);

	Superposition(int start);

	Superposition &operator*=(uint64_t n);

	void step(Superposition &other);
	uint64_t getWins() const;
	bool playing() const;

private:
	uint64_t arriving(int p, int s) const;

	uint64_t states[points][spaces] {};
	uint64_t wins {};
	uint64_t size {};
};

ostream &operator<<(ostream &out, const Superposition &player) {
	out << "       ";
	for (int s = 1; s <= spaces; ++s)
		out << setw(6) << s;
	out << endl;
	out << "     +-";
	for (int s = 1; s <= spaces; ++s)
		out << "------";
	out << endl;
	for (int p = points - 1; p >= 0; --p) {
		out << setw(4) << p << " | ";
		for (int s = 0; s < spaces; ++s)
			out << setw(6) << player.states[p][s];
		out << endl;
	}
	out << player.size << " positions still in play" << endl;
	out << player.wins << " wins so far" << endl;
	return out;
}

constexpr int came_from(int space, int roll) {
	return (space + spaces - roll) % 10;
}

Superposition::Superposition(int start) {
	++states[0][start - 1];
	++size;
}

Superposition &Superposition::operator*=(uint64_t n) {
	for (int p = 0; p < points; ++p)
		for (int s = 0; s < spaces; ++s)
			states[p][s] *= n;
	size *= n;
	return *this;
}

uint64_t Superposition::arriving(int p, int s) const {
	p -= s + 1;
	if (p < 0)
		return 0;

	uint64_t n = 0;
	for (int a = 1; a <= 3; ++a) {
		int from = came_from(s, a);
		for (int b = 1; b <= 3; ++b) {
			int from2 = came_from(from, b);
			for (int c = 1; c <= 3; ++c) {
				int from3 = came_from(from2, c);
				n += states[p][from3];
			}
		}
	}
	return n;
}

void Superposition::step(Superposition &other) {
	uint64_t fin = 0;
	for (int p = points; p < points + spaces; ++p)
		for (int s = p - points; s < spaces; ++s)
			fin += arriving(p, s);
	wins += fin * other.size;

	size = 0;
	for (int p = points - 1; p >= 0; --p)
		for (int s = 0; s < spaces; ++s)
			size += states[p][s] = arriving(p, s);
}

uint64_t Superposition::getWins() const {
	return wins;
}

bool Superposition::playing() const {
	return size;
}

int main() {
	Superposition p1(10);
	Superposition p2(8);
	while (p1.playing() || p2.playing()) {
		p1.step(p2);
		p2.step(p1);
	}
	cout << max(p1.getWins(), p2.getWins()) << endl;
	return EXIT_SUCCESS;
}
