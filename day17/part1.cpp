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

int max_y_achieved(int v) {
	return v > 0 ? v * (v + 1) / 2 : 0;
}

// assume ymax <= 0, i.e. target is below us
bool hits_target_y(int v) {
	if (v > 0)
		v = -v; // what goes up, must come down
	int y = 0;
	while (y > ymax)
		y += v--;
	return y >= ymin;
}

int main() {
	int y = 0;
	for (int v = 1; v < -ymin; ++v)
		if (hits_target_y(v))
			y = max(y, max_y_achieved(v));
	cout << y << endl;
	return EXIT_SUCCESS;
}
