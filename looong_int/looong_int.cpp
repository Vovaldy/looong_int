#include <iostream>
#include "big_integer.hpp"
//#include "PrimeNumbers.h"
using namespace std;





int main() {
	setlocale(LC_ALL, "Russian");
	big_integer bio("27062003"), bi2 = 811;
	//cout << bio.pow(bi2) << endl << endl;

	//
	//cout << "2^n - 1" << endl;
	//cout << bi.pow(n) - 1 << endl << endl;
	//cout << "2^(n-1) + 1" << endl;
	//big_integer bi3("12323456789087654356123456789876543212345678987654321234567898765432");
	//big_integer bi4("987654321987654323456789");
	//cout << "Peremnojenie\n" << bi3 * bi4 << endl;

	big_integer bi("2");
	cout << "Введите n" << endl;
	long n;
	//cin >> n;
	//cout << bi.pow(n) << endl << endl;

	//big_integer start = bi.pow(n - 1) + 1;
	//big_integer end = bi.pow(n) - 1;*/


	//cout << 5 % 13;
	//big_integer candidate = my_rand(end);
	cout  << endl << bi.getBigPrime(bi.pow(100));
	cout << endl;
	//big_integer random = min + (rand() % (max - min + 1));
	/*Prime obj;
	cout << endl <<obj.mulmod(5, 5, 13)<<endl;*/
	//std::cout << min.getBigPrime() << std::endl;
}
