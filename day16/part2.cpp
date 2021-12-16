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

using Bits = vector<uint8_t>;

const string hexdigits = "0123456789ABCDEF";

const string operators[] = { "+", "×", "min", "max", "literal", ">", "<", "=" };
enum { SUM, PRODUCT, MIN, MAX, LITERAL, GT, LT, EQ };

template<class Iterator>
Bits hex_to_bits(Iterator first, Iterator last) {
	Bits v;
	while (first != last) {
		auto n = hexdigits.find(*first);
		v.push_back((n >> 3) & 1);
		v.push_back((n >> 2) & 1);
		v.push_back((n >> 1) & 1);
		v.push_back((n >> 0) & 1);
		++first;
	}
	return v;
}

template<class Iterator>
uint64_t parse_number(Iterator first, Iterator last) {
	uint64_t n = 0;
	while (first != last) {
		n <<= 1;
		n += *first++;
	}
	return n;
}

istream &operator>>(istream &in, Bits &packet) {
	string s;
	in >> s;
	packet.clear();
	for (char digit: s) {
		auto n = hexdigits.find(digit);
		packet.push_back((n >> 3) & 1);
		packet.push_back((n >> 2) & 1);
		packet.push_back((n >> 1) & 1);
		packet.push_back((n >> 0) & 1);
	}
	return in;
}

ostream &operator<<(ostream &out, const Bits &packet) {
	for (auto i = packet.cbegin(); i != packet.cend(); i += 4)
		out << hexdigits[parse_number(i, i + 4)];
	return out;
}

template<class Iterator>
uint64_t parse_version(const Iterator &i) {
	return parse_number(i, i + 3);
}

template<class Iterator>
void skip_version(Iterator &i) {
	i += 3;
}

template<class Iterator>
uint64_t parse_type_id(const Iterator &i) {
	return parse_number(i, i + 3);
}

template<class Iterator>
void skip_type_id(Iterator &i) {
	i += 3;
}

template<class Iterator>
uint64_t parse_literal_value(const Iterator &i) {
	Iterator j = i;
	Bits v;
	for (;;) {
		v.insert(v.end(), j + 1, j + 5);
		if (!*j)
			break;
		j += 5;
	}
	return parse_number(v.begin(), v.end());
}

template<class Iterator>
void skip_literal_value(Iterator &i) {
	while (*i)
		i += 5;
	i += 5;
}

template<class Iterator>
uint64_t parse_length_type_id(const Iterator &i) {
	return *i;
}

template<class Iterator>
void skip_length_type_id(Iterator &i) {
	++i;
}

template<class Iterator>
void skip_length_type_0(Iterator &i) {
	Iterator tmp = i;
	i += 15;
	uint64_t len = parse_number(tmp, i);
	i += len;
}

template<class Iterator>
void skip_packet(Iterator &i);

template<class Iterator>
void skip_length_type_1(Iterator &i) {
	Iterator tmp = i;
	i += 11;
	uint64_t len = parse_number(tmp, i);
	while (len) {
		skip_packet(i);
		--len;
	}
}

template<class Iterator>
void skip_packet(Iterator &i) {
	skip_version(i);

	uint64_t type_id = parse_type_id(i);
	skip_type_id(i);

	if (type_id == 4) {
		skip_literal_value(i);
		return;
	}

	uint8_t length_type = *i++;
	if (!length_type)
		skip_length_type_0(i);
	else
		skip_length_type_1(i);
}

template<class Iterator>
void skip_length_header(Iterator &i) {
	uint8_t length_type = *i++;
	i += length_type ? 11 : 15;
}

template<class Iterator>
uint64_t value(Iterator i) {
	Iterator last = i;
	skip_packet(last);

	skip_version(i);

	uint64_t type_id = parse_type_id(i);
	skip_type_id(i);
	if (type_id != LITERAL)
		skip_length_header(i);

	uint64_t result, left, right;
	switch (type_id) {

	case SUM:
		result = 0;
		while (i != last) {
			result += value(i);
			skip_packet(i);
		}
		break;

	case PRODUCT:
		result = 1;
		while (i != last) {
			result *= value(i);
			skip_packet(i);
		}
		break;

	case MIN:
		result = numeric_limits<uint64_t>::max();
		while (i != last) {
			result = min(result, value(i));
			skip_packet(i);
		}
		break;
		
	case MAX:
		result = numeric_limits<uint64_t>::min();
		while (i != last) {
			result = max(result, value(i));
			skip_packet(i);
		}
		break;

	case LITERAL:
		result = parse_literal_value(i);
		skip_literal_value(i);
		break;

	case GT:
		left = value(i);
		skip_packet(i);

		right = value(i);
		skip_packet(i);

		result = left > right;
		break;

	case LT:
		left = value(i);
		skip_packet(i);

		right = value(i);
		skip_packet(i);

		result = left < right;
		break;

	case EQ:
		left = value(i);
		skip_packet(i);

		right = value(i);
		skip_packet(i);

		result = left == right;
		break;
	}

	return result;
}

template<class Iterator>
void pretty(ostream &out, Iterator i, string padding = "") {
	out << padding << "Value:   " << value(i) << endl;

	Iterator last = i;
	skip_packet(last);

	//out << padding << "Version: " << parse_version(i) << endl;
	skip_version(i);

	uint64_t type_id = parse_type_id(i);
	skip_type_id(i);

	if (type_id != LITERAL) {
		out << padding << "Type:    " << operators[type_id] << endl;
		skip_length_header(i);
		while (i != last) {
			pretty(out, i, padding + " ");
			skip_packet(i);
		}
	}
}

int main() {
	Bits packet;
	while (cin >> packet) {
		//cout << packet << endl;
		//pretty(cout, packet.begin());
		//cout << endl;
		cout << value(packet.begin()) << endl;
	}
	return EXIT_SUCCESS;
}
