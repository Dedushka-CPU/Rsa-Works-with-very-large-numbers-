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
    BigNum() : num("0") {}

    BigNum(const string& value) : num(value) {}

    void Print() const {
        cout << num;
    }

    bool isSmaller(const string& str1, const string& str2) const {
        if (str1.length() < str2.length()) return true;
        if (str1.length() > str2.length()) return false;
        return str1 < str2;
    }

    BigNum operator+(const BigNum& str) const {
        string result;
        int carry = 0;
        string n1 = num;
        string n2 = str.num;
        std::reverse(n1.begin(), n1.end());
        std::reverse(n2.begin(), n2.end());

        int maxLen = std::max(n1.size(), n2.size());
        n1.resize(maxLen, '0');
        n2.resize(maxLen, '0');

        for (int i = 0; i < maxLen; i++) {
            int digitSum = (n1[i] - '0') + (n2[i] - '0') + carry;
            carry = digitSum / 10;
            result.push_back((digitSum % 10) + '0');
        }

        if (carry) {
            result.push_back(carry + '0');
        }
        std::reverse(result.begin(), result.end());

        return BigNum(result);
    }

    string Minus(const string& str1, const string& str2) const {
        string n1 = str1;
        string n2 = str2;
        string result;

        bool isNegative = false;

        if (isSmaller(n1, n2)) {
            std::swap(n1, n2);
            isNegative = true;
        }

        std::reverse(n1.begin(), n1.end());
        std::reverse(n2.begin(), n2.end());

        n2.resize(n1.size(), '0');

        int borrow = 0;
        for (size_t i = 0; i < n1.size(); i++) {
            int sub = (n1[i] - '0') - (n2[i] - '0') - borrow;
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(sub + '0');
        }

        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }

        std::reverse(result.begin(), result.end());

        if (isNegative) {
            result = "-" + result;
        }

        return result.empty() ? "0" : result;
    }

    BigNum operator-(const BigNum& str) const {
        string n1 = num;
        string n2 = str.num;

        if (isSmaller(n1, n2)) {
            return BigNum("0"); 
        }

        string result;
        int borrow = 0;

        std::reverse(n1.begin(), n1.end());
        std::reverse(n2.begin(), n2.end());

        n2.resize(n1.size(), '0');

        for (size_t i = 0; i < n1.size(); i++) {
            int sub = (n1[i] - '0') - (n2[i] - '0') - borrow;
            if (sub < 0) {
                sub += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back(sub + '0');
        }

        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }

        std::reverse(result.begin(), result.end());
        return BigNum(result);
    }

    friend BigNum operator%(const BigNum& dividend, const BigNum& divisor) {
        BigNum dividendCopy = dividend;  
        BigNum divisorCopy = divisor; 
        BigNum x1=dividendCopy/divisorCopy;
        BigNum res=x1*divisorCopy;
        res=dividendCopy-res;
        return BigNum(res);
    }

    bool operator!=(const BigNum& other) const {
        return num != other.num;
    }
    
    bool operator==(const BigNum& other) const {
        return num == other.num;
    }
    
    bool operator>(const BigNum& other) const {
        return num > other.num;
    }
    
    static BigNum Nod(BigNum a, BigNum b) {
        while (b != BigNum("0")) {
            BigNum temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    BigNum operator/(const BigNum& divisor) const {
        if (divisor == BigNum("0")) {
            throw std::invalid_argument("Cannot divide by zero");
        }

        string quotient = "";
        string remainder = "";

        for (char digit : num) {
            if(remainder=="0"){
                remainder = digit; 
            }else{
                remainder += digit; 
            }
            
            int count = 0;

            if (isSmaller(remainder, divisor.num)) {
                if (!quotient.empty()) {
                    quotient += '0'; 
                }
                continue; 
            }

            while (!isSmaller(remainder, divisor.num)) {
                remainder = Minus(remainder, divisor.num);
                count++;
                }

            if (remainder == "0" ) {
                quotient += std::to_string(count);
                remainder = "0";
            } else {
                quotient += std::to_string(count);
            }
        }

        while (quotient.length() > 1 && quotient[0] == '0') {
            quotient.erase(0, 1);
        }

        return BigNum(quotient.empty() ? "0" : quotient);
    }

    BigNum operator*(const BigNum& other) const {
        string num1 = num;
        string num2 = other.num;
        int len1 = num1.size();
        int len2 = num2.size();

        string result(len1 + len2, '0');

        for (int i = len1 - 1; i >= 0; --i) {
            int carry = 0;
            int digit1 = num1[i] - '0';

            for (int j = len2 - 1; j >= 0; --j) {
                int digit2 = num2[j] - '0';
                int sum = (digit1 * digit2) + (result[i + j + 1] - '0') + carry;
                carry = sum / 10;
                result[i + j + 1] = (sum % 10) + '0';
            }
            result[i] += carry;
        }

        size_t start = result.find_first_not_of('0');
        if (start != string::npos) {
            return BigNum(result.substr(start));
        } else {
            return BigNum("0");
        }
    }

    static BigNum PowNum(BigNum base, BigNum exp, BigNum mod) {
        BigNum res("1");
        while (exp > BigNum("0")) {
            if (exp.num.back() % 2 == 1) {
                res = (res * base) % mod;
            }
            base = (base * base) % mod;
            exp = exp / BigNum("2");
        }
        return res;
    }

    char toASCII() const {
        int asciiValue = std::stoi(num);  
        return static_cast<char>(asciiValue);  
    }
};

#endif // RSA_H
