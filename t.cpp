#include "ukko.hpp"
using namespace std;

int func(int x, int y) {
	return x+y;
}

int main() {
	ukko::call(func, 1, 2);
}
