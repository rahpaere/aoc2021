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

using Graph = map<string, set<string>>;

istream &operator>>(istream &in, Graph &g) {
	string line;
	while (getline(in, line)) {
		auto sep = line.find('-');
		string src = line.substr(0, sep);
		string dst = line.substr(sep + 1);
		g[src].insert(dst);
		g[dst].insert(src);
	}
	return in;
}

bool small(const string &cave) {
	return all_of(cave.begin(), cave.end(), [](char c) { return islower(c); });
}

bool revisits_small(const vector<string> &path) {
	vector<string> visited { path };
	sort(visited.begin(), visited.end());
	for (auto it = visited.begin() + 1; it != visited.end(); ++it)
		if (small(*it) && *it == *(it - 1))
			return true;
	return false;
}

bool valid(const string &cave, vector<string> &path) {
	if (cave == "start")
		return false;
	if (!small(cave) || find(path.begin(), path.end(), cave) == path.end())
		return true;
	return !revisits_small(path);
}

unsigned count_paths(Graph g, vector<string> &path) {
	if (path.back() == "end")
		return 1;
	unsigned total = 0;
	for (const auto &cave: g[path.back()]) {
		if (valid(cave, path)) {
			path.push_back(cave);
			total += count_paths(g, path);
			path.pop_back();
		}
	}
	return total;
}

unsigned count_paths(Graph g) {
	vector<string> path { "start" };
	return count_paths(g, path);
}

int main() {
	Graph g;
	cin >> g;
	cout << count_paths(g) << endl;
	return EXIT_SUCCESS;
}
