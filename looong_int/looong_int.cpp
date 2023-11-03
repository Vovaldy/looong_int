#include <iostream>
#include "big_integer.hpp"
using namespace std;


int main() {
	big_integer bi("2"), bi2 = 100;
	//cout << bi.pow(1000) << endl << endl;
	long n;
	cout << "Введите n" << endl;
	cin >> n;
	
	cout << "2^n - 1" << endl;
	cout << bi.pow(n) - 1 << endl << endl;
	cout << "2^(n-1) + 1" << endl;
	cout << bi.pow(n-1) + 1 << endl << endl;
}
