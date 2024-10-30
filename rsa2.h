#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>

using std::cout;
using std::string;

class BigNum {
private:
    string num;

public:
    BigNum() : num("0") {}

    BigNum(const string& value) : num(value) {}

    void Print() const {
        cout << num;
    }
    //проверка что меньше работает
    bool isSmaller(const string& str1, const string& str2) const {
        if (str1.length() < str2.length()) return true;
        if (str1.length() > str2.length()) return false;
        return str1 < str2;
    }
    //оператор+ работает
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
    //функция минус работает
    string Minus(const string& str1, const string& str2) const {
        // Копируем входные строки
        string n1 = str1;
        string n2 = str2;
        string result;

        // Определяем, требуется ли перевести в обратный порядок
        bool isNegative = false;

        // Проверяем, если n1 меньше n2, и меняем порядок
        if (isSmaller(n1, n2)) {
            std::swap(n1, n2);
            isNegative = true; // Если n1 меньше, результат будет отрицательным
        }

        // Реверсируем строки для выполнения вычитания
        std::reverse(n1.begin(), n1.end());
        std::reverse(n2.begin(), n2.end());

        // Увеличиваем длину n2 до длины n1, заполняя нулями
        n2.resize(n1.size(), '0');

        int borrow = 0; // Переменная для хранения займа
        for (size_t i = 0; i < n1.size(); i++) {
            // Вычисляем разницу
            int sub = (n1[i] - '0') - (n2[i] - '0') - borrow;
            if (sub < 0) {
                sub += 10; // Если результат меньше 0, добавляем 10
                borrow = 1; // Устанавливаем заем
            } else {
                borrow = 0; // Нет займа
            }
            result.push_back(sub + '0'); // Добавляем результат
        }

        // Убираем ведущие нули
        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }

        std::reverse(result.begin(), result.end()); // Реверсируем результат

        // Если результат отрицательный, добавляем знак минус
        if (isNegative) {
            result = "-" + result;
        }

        return result.empty() ? "0" : result; // Если результат пустой, возвращаем "0"
    }

    //оператор- работает
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
    //оператор% работает
    friend BigNum operator%(const BigNum& dividend, const BigNum& divisor) {
        // Проверка деления на ноль
        if (divisor == BigNum("0")) {
            throw std::invalid_argument("Division by zero");
        }

        BigNum dividendCopy = dividend;  // Создаем копию делимого
        BigNum divisorCopy = divisor;     // Создаем копию делителя

        // Если делимое меньше делителя, возвращаем его
        if (dividendCopy.isSmaller(dividendCopy.num, divisorCopy.num)) {
            return dividendCopy;  
        }

        string remainder = "";
        for (char digit : dividendCopy.num) {
            remainder += digit;  // Добавляем текущую цифру к остатку
            
            // Уменьшаем остаток до тех пор, пока он больше или равен делителю
            while (!divisorCopy.isSmaller(remainder, divisorCopy.num)) {
                remainder = dividendCopy.Minus(remainder, divisorCopy.num);
            }
        }

        // Создаем и возвращаем объект BigNum с остатком
        return BigNum(remainder.empty() ? "0" : remainder);
    }
    //оператор!= работает
    bool operator!=(const BigNum& other) const {
        return num != other.num;
    }
    bool operator==(const BigNum& other) const {
        return num == other.num;
    }
    bool operator>(const BigNum& other) const{
        return num>other.num;
    }
    //Нод работает
    static BigNum Nod(BigNum a, BigNum b) {
        while (b != BigNum("0")) {
            BigNum temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    //Оператор/ вроде
    BigNum operator/(const BigNum& divisor) const {
    if (divisor == BigNum("0")) {
        throw std::invalid_argument("Nelzya delit na 0");
    }

    string quotient = "";
    string remainder = "";

    for (char digit : num) {
        remainder += digit; // Добавляем текущую цифру

        // Находим, сколько раз делитель помещается в остаток
        int count = 0;
        while (!isSmaller(remainder, divisor.num)) {
            remainder = Minus(remainder, divisor.num); // Вычитаем делитель
            count++;
        }

        // Добавляем найденное значение к частному
        quotient += std::to_string(count);
    }

    // Убираем ведущие нули
    while (quotient.length() > 1 && quotient[0] == '0') {
        quotient.erase(0, 1);
    }

    return BigNum(quotient.empty() ? "0" : quotient); 
}

    //Оператор* вроде тоже работает
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
                carry = sum / 10; // Определяем перенос
                result[i + j + 1] = (sum % 10) + '0'; // Записываем результат
            }
            result[i] += carry; // Добавляем оставшийся перенос
        }

        size_t start = result.find_first_not_of('0');
        if (start != string::npos) {
            return BigNum(result.substr(start)); // Возвращаем результат без ведущих нулей
        } else {
            return BigNum("0"); // Если результат ноль
        }
    }

    //Вроде бы работает
    static BigNum PowNum(BigNum base, BigNum exp, BigNum mod) {
    BigNum res("1"); // Инициализация результата как 1
    while (exp > BigNum("0")) {
        if (exp.num.back() % 2 == 1) { // Проверяем последний бит
            res = (res * base) % mod; // Если нечетный, умножаем результат на основание
        }
        base = (base * base) % mod; // Возводим основание в квадрат
        exp = exp / BigNum("2"); // Делим экспоненту на 2
    }
    return res;
    }

    char toASCII() const{
    // Предполагается, что num содержит строку с числом в десятичном формате
    int asciiValue = std::stoi(num);  // Преобразуем строку в целое число
    return static_cast<char>(asciiValue);  // Преобразуем в символ ASCII
}
};

#endif // RSA_H
