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

class Seafloor {
public:
	using Location = pair<vector<string>::size_type, string::size_type>;
	using Locations = vector<Location>;

	Locations check(char herd, const Location &step) const;
	void forward(const Locations &from, const Location &step);

	friend istream &operator>>(istream &in, Seafloor &s);
	friend ostream &operator<<(ostream &out, const Seafloor &s);

private:
	static const char empty = '.';

	Location destination(const Location &l, const Location &step) const;
	bool empty_at(const Location &l) const;
	bool mobile(char herd, const Location &l, const Location &step) const;

	vector<string> floor;
	string::size_type width;
	vector<string>::size_type height;
};

auto Seafloor::destination(const Location &l, const Location &step) const
								-> Location {
	return { (l.first + step.first) % height,
		 (l.second + step.second) % width };
}

bool Seafloor::mobile(char herd, const Location &l, const Location &step) const {
	Location d = destination(l, step);
	return     floor[l.first][l.second] == herd
		&& floor[d.first][d.second] == empty;
}

auto Seafloor::check(char herd, const Location &step) const -> Locations {
	Locations m;
	Location l;
	for (l.first = 0; l.first < height; ++l.first)
		for (l.second = 0; l.second < width; ++l.second)
			if (mobile(herd, l, step))
				m.push_back(l);
	return m;
}

void Seafloor::forward(const Locations &from, const Location &step) {
	for (const auto &l: from) {
		Location d = destination(l, step);
		swap(floor[l.first][l.second], floor[d.first][d.second]);
	}
}

istream &operator>>(istream &in, Seafloor &s) {
	s.floor.clear();
	copy(istream_iterator<string>(in), {}, back_inserter(s.floor));
	s.height = s.floor.size();
	s.width = s.floor.back().size();
	return in;
}

ostream &operator<<(ostream &out, const Seafloor &s) {
	copy(s.floor.cbegin(), s.floor.cend(), ostream_iterator<string>(out, "\n"));
	return out;
}

int main() {
	Seafloor s;
	cin >> s;

	int steps = 0;
	for (;;) {
		auto emoves = s.check('>', { 0, 1 });
		s.forward(emoves, { 0, 1 });
		auto smoves = s.check('v', { 1, 0 });
		s.forward(smoves, { 1, 0 });
		++steps;
		if (emoves.empty() && smoves.empty())
			break;
	}

	cout << steps << endl;
	return EXIT_SUCCESS;
}
