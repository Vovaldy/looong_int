#include <vector>
#include <string>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <bitset>
//#include "PrimeNumbers.h"
class big_integer {
    // основание системы счисления (1 000 000 000)
    static const int BASE = 1000000000;

    // внутреннее хранилище числа
    std::vector<int> _digits;

    // знак числа
    bool _is_negative;

    void _remove_leading_zeros();
    void _shift_right();

public:
    // класс-исключение, бросаемое при делении на ноль
    class divide_by_zero : public std::exception {  };

    big_integer();
    big_integer(std::string);
    big_integer(signed char);
    big_integer(unsigned char);
    big_integer(signed short);
    big_integer(unsigned short);
    big_integer(signed int);
    big_integer(unsigned int);
    big_integer(signed long);
    big_integer(unsigned long);
    big_integer(signed long long);
    big_integer(unsigned long long);

    friend std::ostream& operator <<(std::ostream&, const big_integer&);
    operator std::string() const;
    const big_integer operator +() const;
    const big_integer operator -() const;
    const big_integer operator ++();
    const big_integer operator ++(int);
    const big_integer operator --();
    const big_integer operator --(int);
    friend bool operator ==(const big_integer&, const big_integer&);
    friend bool operator <(const big_integer&, const big_integer&);
    friend bool operator !=(const big_integer&, const big_integer&);
    friend bool operator <=(const big_integer&, const big_integer&);
    friend bool operator >(const big_integer&, const big_integer&);
    friend bool operator >=(const big_integer&, const big_integer&);
    friend const big_integer operator +(big_integer, const big_integer&);
    big_integer& operator +=(const big_integer&);
    friend const big_integer operator -(big_integer, const big_integer&);
    big_integer& operator -=(const big_integer&);
    friend const big_integer operator *(const big_integer&, const big_integer&);
    big_integer& operator *=(const big_integer&);
    friend const big_integer operator /(const big_integer&, const big_integer&);
    big_integer& operator /=(const big_integer&);
    friend const big_integer operator %(const big_integer&, const big_integer&);
    big_integer& operator %=(const big_integer&);

    bool odd() const;
    bool even() const;
    const big_integer pow(big_integer) const;

    big_integer mulmod(big_integer, big_integer, big_integer);
    big_integer powMod(big_integer, big_integer, big_integer);

    std::vector<int> first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                    31, 37, 41, 43, 47, 53, 59, 61, 67,
                                    71, 73, 79, 83, 89, 97, 101, 103,
                                    107, 109, 113, 127, 131, 137, 139,
                                    149, 151, 157, 163, 167, 173, 179,
                                    181, 191, 193, 197, 199, 211, 223,
                                    227, 229, 233, 239, 241, 251, 257,
                                    263, 269, 271, 277, 281, 283, 293,
                                    307, 311, 313, 317, 331, 337, 347, 349 };
    big_integer getRandom64(big_integer, big_integer);
    big_integer getLowLevelPrime(big_integer, big_integer);
    bool trialComposite(big_integer a, big_integer evenC, big_integer to_test, int max_div_2);
    bool MillerRabinTest(big_integer to_test, big_integer);
    big_integer getBigPrime(big_integer);
    big_integer getSQRTPrime(big_integer);
};

// создает длинное целое число со значением 0
big_integer::big_integer() {
    this->_is_negative = false;
}

// создает длинное целое число из C++-строки
big_integer::big_integer(std::string str) {
    if (str.length() == 0) {
        this->_is_negative = false;
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            this->_is_negative = true;
        }
        else {
            this->_is_negative = false;
        }

        for (long long i = str.length(); i > 0; i -= 9) {
            if (i < 9)
                this->_digits.push_back(atoi(str.substr(0, i).c_str()));
            else
                this->_digits.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }

        this->_remove_leading_zeros();
    }
}

// удаляет ведущие нули
void big_integer::_remove_leading_zeros() {
    while (this->_digits.size() > 1 && this->_digits.back() == 0) {
        this->_digits.pop_back();
    }

    if (this->_digits.size() == 1 && this->_digits[0] == 0) this->_is_negative = false;
}

// печатает число в поток вывода
std::ostream& operator <<(std::ostream& os, const big_integer& bi) {
    if (bi._digits.empty()) os << 0;
    else {
        if (bi._is_negative) os << '-';
        os << bi._digits.back();
        char old_fill = os.fill('0');
        for (long long i = static_cast<long long>(bi._digits.size()) - 2; i >= 0; --i) os << std::setw(9) << bi._digits[i];
        os.fill(old_fill);
    }

    return os;
}

// сравнивает два числа на равенство
bool operator ==(const big_integer& left, const big_integer& right) {
    if (left._is_negative != right._is_negative) return false;
    if (left._digits.empty()) {
        if (right._digits.empty() || (right._digits.size() == 1 && right._digits[0] == 0)) return true;
        else return false;
    }

    if (right._digits.empty()) {
        if (left._digits.size() == 1 && left._digits[0] == 0) return true;
        else return false;
    }

    if (left._digits.size() != right._digits.size()) return false;
    for (size_t i = 0; i < left._digits.size(); ++i) if (left._digits[i] != right._digits[i]) return false;

    return true;
}

// возвращает копию переданного числа
const big_integer big_integer::operator +() const {
    return big_integer(*this);
}

// возвращает переданное число с другим знаком
const big_integer big_integer::operator -() const {
    big_integer copy(*this);
    copy._is_negative = !copy._is_negative;
    return copy;
}

// проверяет, является ли левый операнд меньше правого
bool operator <(const big_integer& left, const big_integer& right) {
    if (left == right) return false;
    if (left._is_negative) {
        if (right._is_negative) return ((-right) < (-left));
        else return true;
    }
    else if (right._is_negative) return false;
    else {
        if (left._digits.size() != right._digits.size()) {
            return left._digits.size() < right._digits.size();
        }
        else {
            for (long long i = left._digits.size() - 1; i >= 0; --i) {
                if (left._digits[i] != right._digits[i]) return left._digits[i] < right._digits[i];
            }

            return false;
        }
    }
}

// сравнивает два числа на неравенство
bool operator !=(const big_integer& left, const big_integer& right) {
    return !(left == right);
}

// проверяет, является ли левый операнд меньше либо равен правого
bool operator <=(const big_integer& left, const big_integer& right) {
    return (left < right || left == right);
}

// проверяет, является ли левый операнд больше правого
bool operator >(const big_integer& left, const big_integer& right) {
    return !(left <= right);
}

// проверяет, является ли левый операнд больше либо равен правого
bool operator >=(const big_integer& left, const big_integer& right) {
    return !(left < right);
}

// складывает два числа
const big_integer operator +(big_integer left, const big_integer& right) {
    if (left._is_negative) {
        if (right._is_negative) return -(-left + (-right));
        else return right - (-left);
    }
    else if (right._is_negative) return left - (-right);
    int carry = 0;
    for (size_t i = 0; i < std::max(left._digits.size(), right._digits.size()) || carry != 0; ++i) {
        if (i == left._digits.size()) left._digits.push_back(0);
        left._digits[i] += carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] >= big_integer::BASE;
        if (carry != 0) left._digits[i] -= big_integer::BASE;
    }

    return left;
}

// прибавляет к текущему числу новое
big_integer& big_integer::operator +=(const big_integer& value) {
    return *this = (*this + value);
}

// префиксный инкремент
const big_integer big_integer::operator++() {
    return (*this += 1);
}

// преобразует число к строке
big_integer::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

// преобразует signed char к big_integer
big_integer::big_integer(signed char c) {
    if (c < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(c));
}

// преобразует unsigned char к big_integer
big_integer::big_integer(unsigned char c) {
    this->_is_negative = false;
    this->_digits.push_back(c);
}

// преобразует signed short к big_integer
big_integer::big_integer(signed short s) {
    if (s < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(s));
}

// преобразует unsigned short к big_integer
big_integer::big_integer(unsigned short s) {
    this->_is_negative = false;
    this->_digits.push_back(s);
}

// преобразует signed int к big_integer
big_integer::big_integer(signed int i) {
    if (i < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(i) % big_integer::BASE);
    i /= big_integer::BASE;
    if (i != 0) this->_digits.push_back(std::abs(i));
}

// преобразует unsigned int к big_integer
big_integer::big_integer(unsigned int i) {
    this->_digits.push_back(i % big_integer::BASE);
    i /= big_integer::BASE;
    if (i != 0) this->_digits.push_back(i);
}

// преобразует signed long к big_integer
big_integer::big_integer(signed long l) {
    if (l < 0) this->_is_negative = true;
    else this->_is_negative = false;
    this->_digits.push_back(std::abs(l) % big_integer::BASE);
    l /= big_integer::BASE;
    if (l != 0) this->_digits.push_back(std::abs(l));
}

// преобразует unsigned long к big_integer
big_integer::big_integer(unsigned long l) {
    this->_digits.push_back(l % big_integer::BASE);
    l /= big_integer::BASE;
    if (l != 0) this->_digits.push_back(l);
}

// преобразует signed long long к big_integer
big_integer::big_integer(signed long long l) {
    if (l < 0) { this->_is_negative = true; l = -l; }
    else this->_is_negative = false;
    do {
        this->_digits.push_back(l % big_integer::BASE);
        l /= big_integer::BASE;
    } while (l != 0);
}

// преобразует unsigned long long к big_integer
big_integer::big_integer(unsigned long long l) {
    this->_is_negative = false;
    do {
        this->_digits.push_back(l % big_integer::BASE);
        l /= big_integer::BASE;
    } while (l != 0);
}

// постфиксный инкремент
const big_integer big_integer::operator ++(int) {
    *this += 1;
    return *this - 1;
}

// префиксный декремент
const big_integer big_integer::operator --() {
    return *this -= 1;
}

// постфиксный декремент
const big_integer big_integer::operator --(int) {
    *this -= 1;
    return *this + 1;
}

// вычитает два числа
const big_integer operator -(big_integer left, const big_integer& right) {
    if (right._is_negative) return left + (-right);
    else if (left._is_negative) return -(-left + right);
    else if (left < right) return -(right - left);
    int carry = 0;
    for (size_t i = 0; i < right._digits.size() || carry != 0; ++i) {
        left._digits[i] -= carry + (i < right._digits.size() ? right._digits[i] : 0);
        carry = left._digits[i] < 0;
        if (carry != 0) left._digits[i] += big_integer::BASE;
    }

    left._remove_leading_zeros();
    return left;
}

// вычитает из текущего числа новое
big_integer& big_integer::operator -=(const big_integer& value) {
    return *this = (*this - value);
}

// перемножает два числа
const big_integer operator *(const big_integer& left, const big_integer& right) {
    big_integer result;
    result._digits.resize(left._digits.size() + right._digits.size());
    for (size_t i = 0; i < left._digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right._digits.size() || carry != 0; ++j) {
            long long cur = result._digits[i + j] +
                left._digits[i] * 1LL * (j < right._digits.size() ? right._digits[j] : 0) + carry;
            result._digits[i + j] = static_cast<int>(cur % big_integer::BASE);
            carry = static_cast<int>(cur / big_integer::BASE);
            //std::cout << "chetchik " << i << " result " << result<<std::endl;
        }
        //std::cout << "Loading% " << left._digits.size() - 1 << std::endl;
        // рабин Миллер
        //берем каждый символ получаем код в кодировке записываем символы в число
        //число это шифровка
        //реализуем алгоритм RSA 
    }

    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}

// домножает текущее число на указанное
big_integer& big_integer::operator *=(const big_integer& value) {
    return *this = (*this * value);
}

// сдвигает все разряды на 1 вправо (домножает на BASE)
void big_integer::_shift_right() {
    if (this->_digits.size() == 0) {
        this->_digits.push_back(0);
        return;
    }
    this->_digits.push_back(this->_digits[this->_digits.size() - 1]);
    for (size_t i = this->_digits.size() - 2; i > 0; --i) this->_digits[i] = this->_digits[i - 1];
    this->_digits[0] = 0;
}

// делит два числа
const big_integer operator /(const big_integer& left, const big_integer& right) {
    if (right == 0) throw big_integer::divide_by_zero();
    big_integer b = right;
    b._is_negative = false;
    big_integer result, current;
    result._digits.resize(left._digits.size());
    for (long long i = static_cast<long long>(left._digits.size()) - 1; i >= 0; --i) {
        current._shift_right();
        current._digits[0] = left._digits[i];
        current._remove_leading_zeros();
        int x = 0, l = 0, r = big_integer::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            big_integer t = b * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }

        result._digits[i] = x;
        current = current - b * x;
    }

    result._is_negative = left._is_negative != right._is_negative;
    result._remove_leading_zeros();
    return result;
}

// делит текущее число на указанное
big_integer& big_integer::operator /=(const big_integer& value) {
    return *this = (*this / value);
}

// возвращает остаток от деления двух чисел
const big_integer operator %(const big_integer& left, const big_integer& right) {
    big_integer result = left - (left / right) * right;
    if (result._is_negative) result += right;
    return result;
}

// присваивает текущему числу остаток от деления на другое число
big_integer& big_integer::operator %=(const big_integer& value) {
    return *this = (*this % value);
}

// проверяет, является ли текущее число нечетным
bool big_integer::odd() const {
    if (this->_digits.size() == 0) return false;
    return this->_digits[0] & 1;
}

// проверяет, является ли текущее число четным
bool big_integer::even() const {
    return !this->odd();
}

// возводит текущее число в указанную степень
const big_integer big_integer::pow(big_integer n) const {
    big_integer a(*this), result(1);
    while (n != 0) {
        if (n.odd()) result *= a;
        a *= a;
        n /= 2;
        std::cout << "Loading% " << n << std::endl;
    }

    return result;
}
/// <summary>
/// ////////////////////////////////////////////////////////////////////////////////////
/// </summary>
/// <returns></returns>
/// 


big_integer big_integer::getBigPrime(big_integer size) {
    big_integer seed = 1;
    while (true) {
        big_integer candidate = getLowLevelPrime(seed, size);
        std::cout << std::endl << "Кандидат после проверки нижнего уровня - -  " << candidate << std::endl << std::endl;
        candidate = getSQRTPrime(candidate);
        std::cout << std::endl << "Кандидат после проверки корня числа - -  " << candidate << std::endl << std::endl;

        
        //if (MillerRabinTest(candidate, seed))
            //return candidate;
        //seed = seed * rand() * rand() + rand();  // Линейный конгруэнтный метод
        return candidate;
    }
}

big_integer big_integer::getSQRTPrime(big_integer candidate) 
{
    std::cout << std::endl << "Кандидат - -" << std::endl << candidate;
    bool is_prime = true;

    while (true)
    {
        is_prime = true;
        for (big_integer i = 3; i*i < candidate; i=i+2)
        {
            std::cout << i << "||||||||";
            if (candidate % i == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            return candidate;
        candidate += 1;
    }
}

big_integer big_integer::getLowLevelPrime(big_integer seed, big_integer size) {
    big_integer candidate = size;
    std::cout << std::endl << "Кандидат - -" << std::endl << candidate;

    bool is_prime = true;
    while (true)
    {

        std::cout << std::endl << "Кандидат - -" << std::endl << candidate;
        is_prime = true;
        for (int i = 0; i < first_primes.size(); i++) {
            if (candidate == first_primes[i])
                return candidate;

            if (candidate % first_primes[i] == 0) {
                is_prime = false;
                //return 0;
            }
        }
        if (candidate._digits[0] % 2 == 0)
        {
            is_prime = false;
            //return 0;
        }

        if (candidate._digits[0] % 5 == 0)
        {
            is_prime = false;
            //return 0;
        }


        if (is_prime)
            return candidate;
        candidate += 1;
    }

}

/*
Суть метода :
    n = to_test		k = accurasy
    d = evenC		r = max_div_2
    d * 2^r = n - 1

*/
bool big_integer::MillerRabinTest(big_integer to_test, big_integer seed) {
    //Точность
    constexpr int accuracy = 20;

    int max_div_2 = 0;
    big_integer evenC = to_test - 1;
    while (evenC % 2 == 0) {
        evenC = evenC/2;
        max_div_2++;
    }

    // random numbers init
    /*std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> distr(2, to_test);*/
    for (int i = 0; i < accuracy; i++) {
        big_integer a = getRandom64(seed, to_test - 2);
        std::cout << "\n\tNumber a =\t" << a << std::endl;// 3765229820729
        if (trialComposite(a, evenC, to_test, max_div_2)) {
            return true;
        }
    }

    return false;
}

/*
Суть метода :
    n = to_test		a = a - случайно сгенерированное число
    d = evenC		r = max_div_2
    1|(n-1) = pow(a, d) % n		- Производится r раз
*/
bool big_integer::trialComposite(big_integer a, big_integer evenC, big_integer to_test, int max_div_2) {
    
    std::cout << "\n\nChto v stepen" << a << "\t v stepen\t" << evenC << std::endl;
    big_integer x = a.pow(evenC) % to_test;

    if ((to_test - 1) == x)
        return true;
    if (1 == x)
        return true;

    //if (powMod(a, evenC, to_test) == 1)
    //    return false;

    //for (int i = 0; i < max_div_2; i++) {
    //    big_integer temp = static_cast<uint64_t>(1) << i;
    //    if (powMod(a, temp * evenC, to_test) == to_test - 1)
    //        return false;
    //}

    for (int i = 0; i < max_div_2; i++) {
        x = (x * x) % to_test;
        if ((to_test - 1) == x)
            return true;
    }

    return false;
}

/*Суть метода :
        n = n		a = a - случайно сгенерированное число
        d = b
        1 | (n - 1) = pow(a, d) % n - Производится r раз

*/


big_integer big_integer::getRandom64(big_integer seed,big_integer size) {
    // the value need to be 63 bits because you can not using 64 bit values do a^2 which is needed
    big_integer asd(2);
    //std::cout << std::endl << "Seed " << seed  << std::endl;
    //std::cout << std::endl << "Size " << size << std::endl;


    std::cout << std::endl << "Random Numb: " << seed % size << std::endl;
    return seed % size;     // Вернуть значение в диапазоне 0-n
}



big_integer big_integer::powMod(big_integer a, big_integer b, big_integer n) {
    big_integer x = 1;

    a %= n;

    while (b > 0) {
        if (b % 2 == 1) {
            x = mulmod(x, a, n); // multiplying with base
        }
        a = mulmod(a, a, n); // squaring the base
        b = b / 2;
    }
    return x % n;
}

big_integer big_integer::mulmod(big_integer a, big_integer b, big_integer m) {
    big_integer res = 0;

    while (a != 0) {
        if (1) {

            res = (res + b) % m;
        }
        a = a / 1;
        b = (b * 2) % m;
    }
    std::cout << std::endl;
    return res;
}
