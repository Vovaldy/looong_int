#pragma once
// 64 bits is maximum you can get in c++ so it's implemented to do so
// you can edit constexpr var in getRandom64() to get lower amount of bits
#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <bitset>

class Prime
{
public:
	
	uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m);
	uint64_t powMod(uint64_t a, uint64_t b, uint64_t n);
	std::vector<int> first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
									31, 37, 41, 43, 47, 53, 59, 61, 67,
									71, 73, 79, 83, 89, 97, 101, 103,
									107, 109, 113, 127, 131, 137, 139,
									149, 151, 157, 163, 167, 173, 179,
									181, 191, 193, 197, 199, 211, 223,
									227, 229, 233, 239, 241, 251, 257,
									263, 269, 271, 277, 281, 283, 293,
									307, 311, 313, 317, 331, 337, 347, 349 };
	uint64_t getRandom64();
	uint64_t getLowLevelPrime();
	bool trialComposite(uint64_t a, uint64_t evenC, uint64_t to_test, int max_div_2);
	bool MillerRabinTest(uint64_t to_test);
	uint64_t getBigPrime();
private:

};


