#include "traits.hpp"
#include <typeinfo>
#include <iostream>
#include <exception>
using namespace std;

int func(int x, int y) {
	return x+y;
}

int main() {
	typedef traits<decltype(func)>::ret T;
	cout<<typeid(T).name()<<'\n';
	cout<<typeid(decltype(func)).name()<<'\n';
	bad_exception e;
	cout<<e.what()<<'\n';
}
