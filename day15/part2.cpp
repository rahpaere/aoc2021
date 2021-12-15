#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

const unsigned REP = 5;

using Grid = vector<vector<unsigned>>;
using Coord = pair<Grid::size_type, Grid::size_type>;

struct Step {
	unsigned cost;
	Coord pos;

	bool operator>(const Step &other) const { return cost > other.cost; };
};

class Cavern {
public:
	void explore();
	unsigned lowest_risk() const;

	friend istream &operator>>(istream &, Cavern &);
	friend ostream &operator<<(ostream &, const Cavern &);

private:
	Step make_step(const Step &, const Coord &) const;
	vector<Step> neighbors(const Step &) const;
	unsigned get_risk(const Coord &) const;
	unsigned heuristic(const Step &) const;

	Grid risk;
	map<Coord, unsigned> cost;
	Coord start;
	Coord exit;
};

istream &operator>>(istream &in, Cavern &c) {
	c.risk.clear();

	string line;
	while (getline(in, line)) {
		c.risk.emplace_back();
		transform(line.begin(), line.end(),
			  back_inserter(c.risk.back()),
		          [](char c) { return c - '0'; });
	}

	c.start = { 0, 0 };
	c.exit = { c.risk.size() * REP - 1, c.risk.back().size() * REP - 1 };

	return in;
}

ostream &operator<<(ostream &out, const Cavern &c) {
	Coord pos;
	for (pos.first = 0; pos.first < c.risk.size() * REP; ++pos.first) {
		Grid::size_type s = c.risk[pos.first % c.risk.size()].size();
		for (pos.second = 0; pos.second < s * REP; ++pos.second)
			out << c.get_risk(pos);
		out << endl;
	}
	return out;
}

Step Cavern::make_step(const Step &from, const Coord &to) const {
	return { cost.at(from.pos) + get_risk(to), to };
}

vector<Step> Cavern::neighbors(const Step &s) const {
	vector<Step> v;
	if (s.pos.first > 0)
		v.push_back(make_step(s, { s.pos.first - 1, s.pos.second }));
	if (s.pos.first < risk.size() * REP - 1)
		v.push_back(make_step(s, { s.pos.first + 1, s.pos.second }));
	if (s.pos.second > 0)
		v.push_back(make_step(s, { s.pos.first, s.pos.second - 1 }));
	if (s.pos.second < risk[s.pos.first % risk.size()].size() * REP - 1)
		v.push_back(make_step(s, { s.pos.first, s.pos.second + 1 }));
	return v;
}

unsigned Cavern::get_risk(const Coord &pos) const {
	Grid::size_type i = pos.first % risk.size();
	Grid::size_type j = pos.second % risk[i].size();
	unsigned delta = (pos.first / risk.size())
			 + (pos.second / risk[i].size());
	return (risk[i][j] - 1 + delta) % 9 + 1;
}

unsigned Cavern::heuristic(const Step &s) const {
	return max(s.pos.first, exit.first)
		- min(s.pos.first, exit.first)
		+ max(s.pos.second, exit.second)
		- min(s.pos.second, exit.second);
}

void Cavern::explore() {
	cost.clear();
	cost[start] = 0;

	priority_queue<Step, vector<Step>, greater<Step>> pq;
	pq.push(Step { 0, start });

	while (!pq.empty()) {
		Step s = pq.top();
		if (s.pos == exit)
			break;
		pq.pop();

		for (auto &t: neighbors(s)) {
			auto it = cost.find(t.pos);
			if (it == cost.end() || t.cost < it->second) {
				cost[t.pos] = t.cost;
				t.cost += heuristic(t);
				pq.push(t);
			}
		}
	}
}

unsigned Cavern::lowest_risk() const {
	return cost.at(exit);
}

int main() {
	Cavern c;
	cin >> c;

	c.explore();
	cout << c.lowest_risk() << endl;

	return EXIT_SUCCESS;
}
