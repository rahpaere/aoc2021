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

const char light = '#';
const char dark = '.';

class Image {
public:
	friend istream &operator>>(istream &, Image &);
	friend ostream &operator<<(ostream &, const Image &);

	int width() const;
	int height() const;
	void enhance(const string & key);
	int count_light() const;

private:
	bool at(int i, int j) const;
	int neighborhood(int i, int j) const;

	vector<string> pixels;
	char background;
};

// assume all rows same width
int Image::width() const {
	return static_cast<int>(pixels[0].size());
}

int Image::height() const {
	return static_cast<int>(pixels.size());
}

bool Image::at(int i, int j) const {
	if (i < 0 || i >= height() || j < 0 || j >= width())
		return background == light;
	return pixels[i][j] == light;
}

int Image::neighborhood(int i, int j) const {
	int n = 0;
	for (int di = -1; di <= 1; ++di)
		for (int dj = -1; dj <= 1; ++dj)
			n = (n << 1) | at(i + di, j + dj);
	return n;
}

void Image::enhance(const string &key) {
	vector<string> enhanced;
	enhanced.reserve(height() + 2);
	for (int i = -1; i < height() + 1; ++i) {
		enhanced.emplace_back();
		enhanced.back().reserve(width() + 2);
		for (int j = -1; j < width() + 1; ++j)
			enhanced.back() += key[neighborhood(i, j)];
	}
	background = key[neighborhood(-2, -2)];
	pixels = move(enhanced);
}

int Image::count_light() const {
	if (background == light)
		return numeric_limits<int>::max();

	int n = 0;
	for (const auto &row: pixels)
		n += count(row.begin(), row.end(), light);
	return n;
}

istream &operator>>(istream &in, Image &image) {
	image.background = dark;
	image.pixels.clear();
	copy(istream_iterator<string>(in), {}, back_inserter(image.pixels));
	return in;
}

ostream &operator<<(ostream &out, const Image &image) {
	for (int i = -1; i < image.height() + 1; ++i) {
		for (int j = -1; j < image.width() + 1; ++j)
			out << image.at(i, j);
		out << endl;
	}
	return out;
}

int main() {
	string key;
	cin >> key;

	Image image;
	cin >> image;

	for (int t = 0; t < 2; ++t)
		image.enhance(key);

	cout << image.count_light() << endl;
	return EXIT_SUCCESS;
}
