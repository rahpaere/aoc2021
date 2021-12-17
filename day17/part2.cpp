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

/* // test
 * const int xmin = 20;
 * const int xmax = 30;
 * const int ymin = -10;
 * const int ymax = -5;
 */

// input
const int xmin = 206;
const int xmax = 250;
const int ymin = -105;
const int ymax = -57;

// assume ymax <= 0, i.e. target is below us
pair<int, int> steps_in_target_y(int v) {
	int y = 0;

	int first = 0;
	while (y > ymax) {
		y += v--;
		++first;
	}

	int last = first;
	while (y >= ymin) {
		y += v--;
		++last;
	}
		
	return { first, last };
}

// assume xmin >= 0
pair<int, int> steps_in_target_x(int v) {
	if (v <= 0) // wrong way!
		return { 0, 0 };

	int x = 0;

	int first = 0;
	while (x < xmin) {
		if (!v) // stops short!
			return { first, first };
		x += v--;
		++first;
	}

	int last = first;
	while (x <= xmax) {
		if (!v) // stops inside target
			return { first, numeric_limits<int>::max() };
		x += v--;
		++last;
	}

	return { first, last };
}

bool hits_target(int vx, int vy) {
	const auto &[ xfirst, xlast ] = steps_in_target_x(vx);
	const auto &[ yfirst, ylast ] = steps_in_target_y(vy);
	return xfirst != xlast && yfirst != ylast
		&& xlast > yfirst && ylast > xfirst;
}

int main() {
	int count = 0;
	for (int vy = ymin; vy <= -ymin; ++vy)
		for (int vx = 1; vx <= xmax; ++vx)
			count += hits_target(vx, vy);
	cout << count << endl;
	return EXIT_SUCCESS;
}
