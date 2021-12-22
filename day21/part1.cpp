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

int main() {
	int pos[2] { 10, 8 };
	int score[2] {};
	int rolls = 0;
	int die = 1;
	int winner;

	for (;;) {
		int player = rolls % 2;
		pos[player] += 3 * die + 3;
		pos[player] = (pos[player] + 9) % 10 + 1;
		die += 3;
		rolls += 3;
		score[player] += pos[player];
		if (score[player] >= 1000) {
			winner = player;
			break;
		}
	}

	cout << score[!winner] << " * " << rolls << " = "
	     << score[!winner] * rolls << endl;
	return EXIT_SUCCESS;
}
