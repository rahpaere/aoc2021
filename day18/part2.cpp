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

class RegularNumber;
class PairNumber;

class SnailNumber {
public:
	static SnailNumber *parse(istream &);

	virtual ~SnailNumber() {}
	virtual ostream &print(ostream &) const = 0;

	virtual RegularNumber *leftmost() = 0;
	virtual RegularNumber *rightmost() = 0;

	virtual int magnitude() = 0;

	virtual SnailNumber *clone() const = 0;

	PairNumber *parent = nullptr;
};

class RegularNumber: public SnailNumber {
public:
	RegularNumber(int v): value(v) {}

	ostream &print(ostream &out) const;

	void accept(SnailNumber *n);
	bool big() const { return value >= 10; }
	SnailNumber *split() const;

	RegularNumber *leftmost() { return this; }
	RegularNumber *rightmost() { return this; }

	int magnitude() { return value; }

	SnailNumber *clone() const { return new RegularNumber(value); }

private:
	int value;
};

class PairNumber: public SnailNumber {
public:
	PairNumber(SnailNumber *l, SnailNumber *r) {
		setLeft(l);
		setRight(r);
	}

	void setLeft(SnailNumber *n) {
		left = n;
		left->parent = this;
	}

	void setRight(SnailNumber *n) {
		right = n;
		right->parent = this;
	}

	PairNumber(const PairNumber &) = delete;
	PairNumber &operator=(const PairNumber &) = delete;
	~PairNumber() { delete left; delete right; }

	ostream &print(ostream &out) const;

	bool deep() const;

	RegularNumber *leftmost() { return left->leftmost(); }
	RegularNumber *rightmost() { return right->rightmost(); }
	RegularNumber *prev();
	RegularNumber *next();

	void explode();
	void reduce();

	int magnitude() { return 3 * left->magnitude() + 2 * right->magnitude(); }

	SnailNumber *clone() const {
		return new PairNumber(left->clone(), right->clone());
	}

private:
	bool reduce_explosions();
	bool reduce_splits();

	SnailNumber *left = nullptr;
	SnailNumber *right = nullptr;
};

SnailNumber *SnailNumber::parse(istream &in) {
	int i;
	if (in >> i)
		return new RegularNumber(i);
	if (in.eof())
		return nullptr;
	in.clear();
	in.ignore(numeric_limits<streamsize>::max(), '[');
	SnailNumber *left = parse(in);
	in.ignore(numeric_limits<streamsize>::max(), ',');
	SnailNumber *right = parse(in);
	in.ignore(numeric_limits<streamsize>::max(), ']');
	return new PairNumber(left, right);
}

ostream &RegularNumber::print(ostream &out) const {
	if (big()) {
		return out << "\033[31m" << value << "\033[0m";
	} else {
		return out << value;
	}
}

void RegularNumber::accept(SnailNumber *n) {
	value += static_cast<RegularNumber *>(n)->value;
}

bool PairNumber::deep() const {
	int depth = 0;
	const PairNumber *tmp = this;
	while (tmp->parent) {
		++depth;
		tmp = tmp->parent;
	}
	return depth >= 4;
}

RegularNumber *PairNumber::prev() {
	PairNumber *tmp = this;
	while (tmp->parent && tmp == tmp->parent->left)
		tmp = tmp->parent;
	tmp = tmp->parent;
	return tmp ? tmp->left->rightmost() : nullptr;
}

RegularNumber *PairNumber::next() {
	PairNumber *tmp = this;
	while (tmp->parent && tmp == tmp->parent->right)
		tmp = tmp->parent;
	tmp = tmp->parent;
	return tmp ? tmp->right->leftmost() : nullptr;
}

SnailNumber *RegularNumber::split() const {
	RegularNumber *left = new RegularNumber(value / 2);
	RegularNumber *right = new RegularNumber((value + 1) / 2);
	return new PairNumber(left, right);
}

ostream &PairNumber::print(ostream &out) const {
	if (deep())
		out << "\033[31m";
	out << '[';
	left->print(out);
	out << ',';
	right->print(out);
	return out << "]\033[0m";
}

void PairNumber::explode() {
	RegularNumber *l = prev();
	if (l)
		l->accept(left);

	RegularNumber *r = next();
	if (r)
		r->accept(right);
}

void PairNumber::reduce() {
	for (;;) {
		//print(cout) << endl;
		if (reduce_explosions())
			continue;
		if (reduce_splits())
			continue;
		break;
	}
	//print(cout) << endl << endl;
}

bool PairNumber::reduce_explosions() {
	PairNumber *pl = dynamic_cast<PairNumber *>(left);
	if (pl) {
		if (pl->deep()) {
			pl->explode();
			setLeft(new RegularNumber(0));
			delete pl;
			return true;
		}
		if (pl->reduce_explosions()) {
			return true;
		}
	}

	PairNumber *pr = dynamic_cast<PairNumber *>(right);
	if (pr) {
		if (pr->deep()) {
			pr->explode();
			setRight(new RegularNumber(0));
			delete pr;
			return true;
		}
		if (pr->reduce_explosions()) {
			return true;
		}
	}

	return false;
}

bool PairNumber::reduce_splits() {
	PairNumber *pl = dynamic_cast<PairNumber *>(left);
	if (pl && pl->reduce_splits())
		return true;

	RegularNumber *rl = dynamic_cast<RegularNumber *>(left);
	if (rl && rl->big()) {
		setLeft(rl->split());
		delete rl;
		return true;
	}

	PairNumber *pr = dynamic_cast<PairNumber *>(right);
	if (pr && pr->reduce_splits())
		return true;

	RegularNumber *rr = dynamic_cast<RegularNumber *>(right);
	if (rr && rr->big()) {
		setRight(rr->split());
		delete rr;
		return true;
	}

	return false;
}

SnailNumber *add(SnailNumber *x, SnailNumber *y) {
	PairNumber *p = new PairNumber(x, y);
	p->reduce();
	return p;
}

int main() {
	vector<SnailNumber *> numbers;
	for (;;) {
		SnailNumber *n = SnailNumber::parse(cin);
		if (!n)
			break;
		numbers.push_back(n);
	}

	int largest = 0;
	for (auto m: numbers) {
		for (auto n: numbers) {
			SnailNumber *sum = add(m->clone(), n->clone());
			largest = max(largest, sum->magnitude());
			delete sum;
		}
	}

	cout << largest << endl;

	for (auto n: numbers)
		delete n;
	return EXIT_SUCCESS;
}
