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

pair<int, int> parse(const string &s) {
	istringstream in { s };
	char comma;
	int x, y;
	in >> x >> comma >> y;
	return { x, y };
}

int reflect(int n, int along) {
	return n < along ? n : 2 * along - n;
}

set<pair<int, int>> read_dots(istream &in) {
	string line;
	set<pair<int, int>> paper;
	while (getline(in, line) && !line.empty())
		paper.insert(parse(line));
	return paper;
}

set<pair<int, int>> fold_horizontally(set<pair<int, int>> paper, int y) {
	set<pair<int, int>> folded;
	for (auto dot: paper)
		folded.insert({ dot.first, reflect(dot.second, y) });
	return folded;
}

set<pair<int, int>> fold_vertically(set<pair<int, int>> paper, int x) {
	set<pair<int, int>> folded;
	for (auto dot: paper)
		folded.insert({ reflect(dot.first, x), dot.second });
	return folded;
}

pair<int, int> bounding_box(set<pair<int, int>> paper) {
	pair<int, int> box { 0, 0 };
	for (auto dot: paper) {
		box.first = max(box.first, dot.first);
		box.second = max(box.second, dot.second);
	}
	return box;
}

void print_dots(set<pair<int, int>> paper) {
	pair<int, int> box = bounding_box(paper);
	for (int j = 0; j <= box.second; ++j) {
		for (int i = 0; i <= box.first; ++i) {
			if (paper.find({ i, j }) != paper.end())
				cout << '#';
			else
				cout << '.';
		}
		cout << endl;
	}
}

int main() {
	set<pair<int, int>> paper = read_dots(cin);

	string instruction;
	while (getline(cin, instruction, '=')) {
		int along;
		cin >> along;
		if (!cin)
			break;
		if (instruction.back() == 'x')
			paper = fold_vertically(paper, along);
		else
			paper = fold_horizontally(paper, along);
		break; // only first
	}

	cout << paper.size() << endl;
	return EXIT_SUCCESS;
}
