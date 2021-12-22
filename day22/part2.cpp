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

class Range1d {
public:
	Range1d() = default;
	Range1d(const Range1d &) = default;
	Range1d(int64_t lo, int64_t hi): lo(lo), hi(hi) {}

	uint64_t size() const;

	bool operator==(const Range1d &other) const;

	bool overlaps(const Range1d &other) const;
	Range1d overlap(const Range1d &other) const;
	vector<Range1d> split(const Range1d &subrange) const;

	friend istream &operator>>(istream &in, Range1d &r);

private:
	int64_t lo, hi;
};

class Range3d {
public:
	Range3d() = default;
	Range3d(const Range3d &) = default;
	Range3d(const Range1d &x, const Range1d &y, const Range1d &z)
		: x(x), y(y), z(z) {}

	uint64_t size() const;

	bool operator==(const Range3d &other) const;

	bool overlaps(const Range3d &other) const;
	Range3d overlap(const Range3d &other) const;
	vector<Range3d> without(const Range3d &subrange) const;

	friend istream &operator>>(istream &in, Range3d &r);

private:
	Range1d x, y, z;
};

uint64_t Range1d::size() const {
	return hi + 1 - lo;
}

uint64_t Range3d::size() const {
	return x.size() * y.size() * z.size();
}

bool Range1d::operator==(const Range1d &other) const {
	return hi == other.hi && lo == other.lo;
}

bool Range3d::operator==(const Range3d &other) const {
	return x == other.x && y == other.y && z == other.z;
}

bool Range1d::overlaps(const Range1d &other) const {
	return other.lo <= hi && lo <= other.hi;
}

bool Range3d::overlaps(const Range3d &other) const {
	return x.overlaps(other.x) && y.overlaps(other.y) && z.overlaps(other.z);
}

Range1d Range1d::overlap(const Range1d &other) const {
	return { max(lo, other.lo), min(hi, other.hi) };
}

Range3d Range3d::overlap(const Range3d &other) const {
	return { x.overlap(other.x), y.overlap(other.y), z.overlap(other.z) };
}

vector<Range1d> Range1d::split(const Range1d &subrange) const {
	vector<Range1d> v;
	if (lo < subrange.lo)
		v.emplace_back(lo, subrange.lo - 1);
	v.emplace_back(subrange);
	if (hi > subrange.hi)
		v.emplace_back(subrange.hi + 1, hi);
	return v;
}

vector<Range3d> Range3d::without(const Range3d &subrange) const {
	vector<Range3d> v;
	for (const auto &xsub: x.split(subrange.x)) {
		for (const auto &ysub: y.split(subrange.y)) {
			for (const auto &zsub: z.split(subrange.z)) {
				Range3d sub { xsub, ysub, zsub };
				if (!(sub == subrange))
					v.push_back(sub);
			}
		}
	}
	return v;
}

istream &operator>>(istream &in, Range1d &r) {
	char dump;
	return in >> dump >> dump >> r.lo >> dump >> dump >> r.hi;
}

istream &operator>>(istream &in, Range3d &r) {
	char comma;
	return in >> r.x >> comma >> r.y >> comma >> r.z;
}

class Steps {
public:
	void insert(bool on, const Range3d &cuboid);
	uint64_t size() const;

private:
	list<Range3d> steps;
};

void Steps::insert(bool on, const Range3d &cuboid) {
	auto next = steps.begin();
	for (auto i = next; i != steps.end(); i = next) {
		++next;
		if (!i->overlaps(cuboid))
			continue;
		auto split = i->without(i->overlap(cuboid));
		steps.insert(next, split.begin(), split.end());
		steps.erase(i);
	}
	if (on)
		steps.emplace_back(cuboid);
}

uint64_t Steps::size() const {
	uint64_t n = 0;
	for (const auto &r: steps)
		n += r.size();
	return n;
}

int main() {
	Steps reboot;

	string on;
	Range3d cuboid;
	while (cin >> on >> cuboid)
		reboot.insert(on == "on", cuboid);

	cout << reboot.size() << endl;
	return EXIT_SUCCESS;
}
