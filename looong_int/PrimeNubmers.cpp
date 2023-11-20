#include "PrimeNumbers.h"

uint64_t Prime::getBigPrime() {
	while (true) {
		uint64_t candidate = getLowLevelPrime();
		if (MillerRabinTest(candidate))
			return candidate;
	}
}

/*
Суть метода :
	n = to_test		k = accurasy
	d = evenC		r = max_div_2
	d * 2^r = n - 1

*/
bool Prime::MillerRabinTest(uint64_t to_test) {
	//Точность
	constexpr int accuracy = 20;

	int max_div_2 = 0;
	uint64_t evenC = to_test - 1;
	while (evenC % 2 == 0) {
		evenC >>= 1;
		max_div_2++;
	}

	// random numbers init
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint64_t> distr(2, to_test);

	for (int i = 0; i < accuracy; i++) {
		uint64_t a = distr(gen);

		if (trialComposite(a, evenC, to_test, max_div_2)) {
			return false;
		}
	}

	return true;
}

/*
Суть метода :
	n = to_test		a = a - случайно сгенерированное число
	d = evenC		r = max_div_2
	1|(n-1) = pow(a, d) % n		- Производится r раз
*/
bool Prime::trialComposite(uint64_t a, uint64_t evenC, uint64_t to_test, int max_div_2) {
	if (powMod(a, evenC, to_test) == 1)
		return false;

	for (int i = 0; i < max_div_2; i++) {
		uint64_t temp = static_cast<uint64_t>(1) << i;
		if (powMod(a, temp * evenC, to_test) == to_test - 1)
			return false;
	}

	return true;
}

/*Суть метода :
		n = n		a = a - случайно сгенерированное число
		d = b			
		1 | (n - 1) = pow(a, d) % n - Производится r раз

*/
uint64_t Prime::powMod(uint64_t a, uint64_t b, uint64_t n) {
	uint64_t x = 1;

	a %= n;

	while (b > 0) {
		if (b % 2 == 1) {
			x = mulmod(x, a, n); // multiplying with base
		}
		a = mulmod(a, a, n); // squaring the base
		b >>= 1;
	}
	return x % n;
}

uint64_t Prime::mulmod(uint64_t a, uint64_t b, uint64_t m) {
	int64_t res = 0;

	while (a != 0) {
		if (a & 1) {

			res = (res + b) % m;
		}
		a >>= 1;
		b = (b << 1) % m;
	}
	std::cout << std::endl;
	return res;
}

uint64_t Prime::getLowLevelPrime() {
	while (true) {
		uint64_t candidate = getRandom64();
		bool is_prime = true;
		for (int i = 0; i < first_primes.size(); i++) {
			if (candidate == first_primes[i])
				return candidate;

			if (candidate % first_primes[i] == 0) {
				is_prime = false;
				break;
			}
		}
		if (is_prime)
			return candidate;
	}
}

uint64_t Prime::getRandom64() {
	// the value need to be 63 bits because you can not using 64 bit values do a^2 which is needed
	constexpr int bits = 63;
	std::bitset<bits> a;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<short> distr(0, 1);

	for (int i = 0; i < bits; i++) {
		a[i] = distr(gen);
	}

	a[0] = 1;
	a[bits - 1] = 1;

	return a.to_ullong();
}


