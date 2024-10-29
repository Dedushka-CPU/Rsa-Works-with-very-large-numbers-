#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

class BigNum {
private:
    std::vector<uint64_t> data;
    static const int BASE_BITS = 64;

public:
    BigNum(const std::string& number) {
        *this = parseString(number);
    }

    BigNum(uint64_t value = 0) {
        if (value > 0) {
            data.push_back(value);
        }
    }
    BigNum(char c) {
        data.push_back(static_cast<uint64_t>(c)); 
    }
    BigNum(int value) {
        if (value < 0) {
            throw std::invalid_argument("Negative value not supported");
        }
        if (value > 0) {
            data.push_back(static_cast<uint64_t>(value)); 
        }
    }
    uint64_t toUInt() const {
        uint64_t result = 0;

        for (size_t i = 0; i < data.size(); ++i) {
            result += data[i] * (1ULL << (i * BASE_BITS)); 
        }

        return result;
    }
    char toChar() const {
        uint64_t num = toUInt(); 

        if (num < 0 || num > 255) {
            throw std::out_of_range("BigNum value out of range for char");
        }

        return static_cast<char>(num);
    }

    BigNum operator+(const BigNum& other) const {
        BigNum result;
        uint64_t carry = 0;
        size_t maxSize = std::max(data.size(), other.data.size());

        for (size_t i = 0; i < maxSize || carry; ++i) {
            uint64_t sum = carry;
            if (i < data.size()) sum += data[i];
            if (i < other.data.size()) sum += other.data[i];

            carry = (sum < carry) ? 1 : 0;
            result.data.push_back(sum);
        }

        return result;
    }

   static BigNum Nod(BigNum a, BigNum b) {
        while (b != BigNum(0)) { 
            BigNum temp = b;
            b = a % b; 
            a = temp;
        }
        return a;
    }

   BigNum operator-(const BigNum& other) const {
        BigNum result;
        uint64_t borrow = 0;

        if (*this == other) {
            return BigNum(0);
        }

        if (*this < other) {
            throw std::invalid_argument("Result would be negative"); 
        }

        for (size_t i = 0; i < data.size(); ++i) {
            uint64_t currentOther = (i < other.data.size()) ? other.data[i] : 0;
            uint64_t diff = data[i] - (borrow + currentOther);
            
            if (data[i] < borrow + currentOther) {
                borrow = 1;
                diff += (1ULL << BASE_BITS); 
            } else {
                borrow = 0;
            }

            result.data.push_back(diff);
        }

        while (result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }

        return result;
    }


    BigNum operator*(const BigNum& other) const {
        BigNum result;
        result.data.resize(data.size() + other.data.size(), 0);

        for (size_t i = 0; i < data.size(); ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < other.data.size() || carry; ++j) {
                __uint128_t product = static_cast<__uint128_t>(data[i]) * (j < other.data.size() ? other.data[j] : 0) + result.data[i + j] + carry;
                result.data[i + j] = static_cast<uint64_t>(product);
                carry = static_cast<uint64_t>(product >> BASE_BITS);
            }
        }

        while (result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }

        return result;
    }
    std::pair<BigNum, uint64_t> divide(const BigNum& divisor) const {
        BigNum quotient;
        BigNum remainder = *this;

        for (int i = data.size() * BASE_BITS - 1; i >= 0; --i) {
            remainder = (remainder * 2);
            if (i < data.size() * BASE_BITS) {
                remainder = remainder + ((data[i / BASE_BITS] >> (i % BASE_BITS)) & 1);
            }
            if (remainder >= divisor) {
                remainder = remainder - divisor;
                quotient = (quotient * 2) + BigNum(1);
            } else {
                quotient = quotient * 2;
            }
        }

        return {quotient, remainder.data.empty() ? 0 : remainder.data[0]};
    }

    bool operator<(const BigNum& other) const {
        if (data.size() != other.data.size())
            return data.size() < other.data.size();
        for (int i = data.size() - 1; i >= 0; --i) {
            if (data[i] != other.data[i])
                return data[i] < other.data[i];
        }
        return false;
    }

    bool operator==(const BigNum& other) const {
        return data == other.data;
    }

    bool operator!=(const BigNum& other) const {
        return !(*this == other);
    }

    bool operator<=(const BigNum& other) const {
        return *this < other || *this == other;
    }

    bool operator>(const BigNum& other) const {
        return other < *this;
    }

    bool operator>=(const BigNum& other) const {
        return !(*this < other);
    }

    static BigNum PowNum(BigNum base, BigNum exp, BigNum mod) {
        BigNum res(1);
        while (exp > BigNum(0)) {
            if (exp.data[0] % 2 == 1) {
                res = (res * base) % mod;
            }
            base = (base * base) % mod;
            exp = exp / BigNum(2); 
        }
        return res;
    }

    BigNum& operator=(const BigNum& other) {
        if (this != &other) { 
            data = other.data;
        }
        return *this; 
    }

    friend BigNum operator/(const BigNum& a, const BigNum& b) {
        if (b.isZero()) {
            throw std::invalid_argument("Division by zero");
        }
        if (a < b) {
            return BigNum(0);
        }

        BigNum res(0);
        BigNum remainder = a;

        while (remainder >= b) {
            remainder = remainder - b; 
            res = res + BigNum(1);     
        }

        return res; 
    }

    friend BigNum operator %(const BigNum& a,const BigNum& b){
        if(a<b){
            return a;
        }
        BigNum remainder = a;

        while (remainder >= b) {
            remainder = remainder - b;
        }
        return remainder;
    }

    void print() const {
        if (data.empty()) {
            std::cout << "0";
            return;
        }

        BigNum temp = *this;
        std::string result;

        while (!temp.isZero()) {
            uint64_t remainder = temp.divideBy10();
            result.push_back(remainder + '0');
        }

        std::reverse(result.begin(), result.end());
        std::cout << result ;
    }
    BigNum parseString(const std::string& number) const {
        BigNum result;
        for (char c : number) {
            result = result * 10 + (c - '0');
        }
        return result;
    }

    bool isZero() const {
        return data.size() == 1 && data[0] == 0;
    }

    uint64_t divideBy10() {
        uint64_t remainder = 0;

        for (int i = data.size() - 1; i >= 0; --i) {
            __uint128_t current = (static_cast<__uint128_t>(remainder) << BASE_BITS) + data[i];
            data[i] = current / 10;
            remainder = current % 10;
        }

        while (data.size() > 1 && data.back() == 0) {
            data.pop_back();
        }

        return remainder;
    }

    
};

#endif // RSA_H
