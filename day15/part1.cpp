#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

using Grid = vector<vector<unsigned>>;

struct Step {
	unsigned cost;
	Grid::size_type row, col;

	bool operator>(const Step &other) const { return cost > other.cost; };
};

class Cavern {
public:
	void explore();
	unsigned lowest_risk() const;

	friend istream &operator>>(istream &in, Cavern &c);

private:
	Step make_step(const Step &, Grid::size_type, Grid::size_type) const;
	vector<Step> neighbors(const Step &s) const;

	Grid risk;
	Grid cost;
	Grid::size_type start_row, start_col;
	Grid::size_type exit_row, exit_col;
};

istream &operator>>(istream &in, Cavern &c) {
	c.risk.clear();
	c.cost.clear();

	string line;
	while (getline(in, line)) {
		c.risk.emplace_back();
		transform(line.begin(), line.end(),
			  back_inserter(c.risk.back()),
		          [](char c) { return c - '0'; });
		c.cost.emplace_back(c.risk.back().size(),
				    numeric_limits<unsigned>::max());
	}
	c.start_row = 0;
	c.start_col = 0;
	c.exit_row = c.risk.size() - 1;
	c.exit_col = c.risk.back().size() - 1;

	return in;
}

Step Cavern::make_step(const Step &from, Grid::size_type row,
					 Grid::size_type col) const {
	return { cost[from.row][from.col] + risk[row][col], row, col };
}

vector<Step> Cavern::neighbors(const Step &s) const {
	vector<Step> v;
	if (s.row > 0)
		v.push_back(make_step(s, s.row - 1, s.col));
	if (s.row < risk.size() - 1)
		v.push_back(make_step(s, s.row + 1, s.col));
	if (s.col > 0)
		v.push_back(make_step(s, s.row, s.col - 1));
	if (s.col < risk[s.row].size() - 1)
		v.push_back(make_step(s, s.row, s.col + 1));
	return v;
}

void Cavern::explore() {
	priority_queue<Step, vector<Step>, greater<Step>> pq;

	cost[start_row][start_col] = 0;
	pq.push(Step { 0, start_row, start_col });

	while (!pq.empty()) {
		Step s = pq.top();
		pq.pop();
		if (s.row == exit_row && s.col == exit_col)
			break;

		for (auto &t: neighbors(s)) {
			if (t.cost < cost[t.row][t.col]) {
				cost[t.row][t.col] = t.cost;
				pq.push(t);
			}
		}
	}
}

unsigned Cavern::lowest_risk() const {
	return cost[exit_row][exit_col];
}

int main() {
	Cavern c;
	cin >> c;
	c.explore();
	cout << c.lowest_risk() << endl;
	return EXIT_SUCCESS;
}
