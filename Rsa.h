#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <stdexcept>

using std::cout;
using std::string;
using std::endl;

class BigNum {
private:
    string num;

public:
    BigNum();
    BigNum(const string& value);   
    
    string toString() const;
    char toASCII() const;
    static BigNum Nod(BigNum a, BigNum b);
    static BigNum PowNum(BigNum base, BigNum exp, BigNum mod);
    bool isSmaller(const string& str1, const string& str2) const;
    string Minus(const string& str1, const string& str2) const;

    BigNum operator+(const BigNum& str) const;
    BigNum operator-(const BigNum& str) const;
    BigNum operator/(const BigNum& divisor) const;
    BigNum operator*(const BigNum& other) const;
    friend BigNum operator%(const BigNum& dividend, const BigNum& divisor);
    bool operator!=(const BigNum& other) const;
    bool operator==(const BigNum& other) const;
    bool operator>(const BigNum& other) const;
    friend std::ostream& operator<<(std::ostream& os, const BigNum& bigNum);
};

#endif // RSA_H
