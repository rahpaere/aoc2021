#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <list>
#include <numeric>
#include <iomanip>
#include <vector>

using namespace std;

struct Point {
	int x;
	int y;
};

struct Line {
	Point start;
	Point end;
};

struct Map {
	static const int width = 1000;
	int loc[width][width] {};
};

istream &operator>>(istream &in, Point &p) {
	in >> p.x;
	in.ignore(); // skip comma
	in >> p.y;
	return in;
}

istream &operator>>(istream &in, Line &l) {
	in >> l.start;
	in.ignore(3); // skip ->
	in >> l.end;
	return in;
}

ostream &operator<<(ostream &out, const Point &p) {
	return out << p.x << ',' << p.y;
}

ostream &operator<<(ostream &out, const Line &l) {
	return out << l.start << " -> " << l.end;
}

ostream &operator<<(ostream &out, const Map &m) {
	for (int j = 0; j < Map::width; ++j) {
		for (int i = 0; i < Map::width; ++i) {
			if (m.loc[i][j])
				out << m.loc[i][j];
			else
				out << '.';
		}
		out << endl;
	}
	return out;
}

void mark(const Line &line, Map &map) {
	if (line.start.x == line.end.x) {
		int stride = line.start.y < line.end.y ? 1 : -1;
		for (int y = line.start.y; y != line.end.y + stride; y += stride)
			++map.loc[line.start.x][y];
	} else if (line.start.y == line.end.y) {
		int stride = line.start.x < line.end.x ? 1 : -1;
		for (int x = line.start.x; x != line.end.x + stride; x += stride)
			++map.loc[x][line.start.y];
	}
}

int main() {
	vector<Line> lines;
	copy(istream_iterator<Line>(cin), {}, back_inserter(lines));
	//copy(begin(lines), end(lines), ostream_iterator<Line>(cout, "\n"));

	Map map {};
	for (const Line &line: lines) {
		mark(line, map);
		//cout << line << endl << map << endl;
	}

	int count = 0;
	for (int i = 0; i < Map::width; ++i)
		for (int j = 0; j < Map::width; ++j)
			if (map.loc[i][j] > 1)
				++count;

	cout << count << endl;
	return EXIT_SUCCESS;
}
