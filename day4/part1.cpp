#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <list>
#include <numeric>
#include <iomanip>

using namespace std;

struct Bingo {
	int numbers[5][5];
	bool marked[5][5];
};

istream &operator>>(istream &in, Bingo &board) {
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j) {
			in >> board.numbers[i][j];
			board.marked[i][j] = false;
		}
	return in;
}

void read_draws(istream &in, list<int> &draws) {
	string line;
	getline(in, line);
	replace(line.begin(), line.end(), ',', ' ');

	istringstream iss { line };
	copy(istream_iterator<int>(iss), {}, back_inserter(draws));
}

void mark(Bingo &board, int n) {
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			if (board.numbers[i][j] == n)
				board.marked[i][j] = true;
}

bool wins(const Bingo &board) {
	int rows[5] {};
	int cols[5] {};
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j) {
			rows[i] += board.marked[i][j];
			cols[j] += board.marked[i][j];
		}
	for (int i = 0; i < 5; ++i)
		if (rows[i] == 5 || cols[i] == 5)
			return true;
	return false;
}

int unmarked(const Bingo &board) {
	int sum = 0;
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			if (!board.marked[i][j])
				sum += board.numbers[i][j];
	return sum;
}

ostream &operator<<(ostream &out, const Bingo &board) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (board.marked[i][j])
				out << "  *";
			else
				out << setw(3) << board.numbers[i][j];
		}
		out << endl;
	}
	return out;
}

int main() {
	list<int> draws;
	read_draws(cin, draws);

	list<Bingo> boards;
	copy(istream_iterator<Bingo>(cin), {}, back_inserter(boards));

	for (int n: draws) {
		//cout << endl << "==== " << n << " ====" << endl;
		for (auto i = boards.begin(); i != boards.end(); ++i) {
			mark(*i, n);
			//cout << board << endl;
			if (wins(*i)) {
				//cout << "We have a winner!" << endl;
				int sum = unmarked(*i);
				cout << sum << " * " << n << " = " << sum * n << endl;
				return EXIT_SUCCESS;
			}
		}
	}
	return EXIT_SUCCESS;
}
