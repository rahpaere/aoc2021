#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

const int width = 12;

template<class Iterator>
bool most_common_bit(Iterator first, Iterator last, int pos) {
	auto n = count_if(first, last, [pos](auto x) { return (x >> pos) & 1; });
	return n * 2 >= distance(first, last);
}

template<class Iterator>
Iterator remove_least_common(Iterator first, Iterator last, int pos) {
	bool b = most_common_bit(first, last, pos);
	return remove_if(first, last, [b, pos](auto x) { return b != ((x >> pos) & 1); });
}

template<class Iterator>
Iterator remove_most_common(Iterator first, Iterator last, int pos) {
	bool b = most_common_bit(first, last, pos);
	return remove_if(first, last, [b, pos](auto x) { return b == ((x >> pos) & 1); });
}

int oxygen_rating(vector<int> readings) {
	int pos = width;
	while (readings.size() > 1 && pos > 0) {
		--pos;
		readings.erase(remove_least_common(readings.begin(),
						   readings.end(),
						   pos),
			       readings.end());
	}
	return readings[0];
}

int carbon_dioxide_rating(vector<int> readings) {
	int pos = width;
	while (readings.size() > 1 && pos > 0) {
		--pos;
		readings.erase(remove_most_common(readings.begin(),
						  readings.end(),
						  pos),
			       readings.end());
	}
	return readings[0];
}

int main() {
	vector<int> readings;

	string line;
	while (cin >> line) {
		int reading {};
		for (char c: line)
			reading = (reading << 1) | (c == '1');
		readings.push_back(reading);
	}

	int x = oxygen_rating(readings);
	int y = carbon_dioxide_rating(readings);
	cout << x << " * " << y << " = " << x * y << endl;

	return EXIT_SUCCESS;
}
