#include <iostream>
#include "Rsa.h"
#include <random>
#include <vector>

void generateKey(BigNum& e, BigNum& d, BigNum& n) {
    std::vector<uint64_t> primes = {
        // список простых чисел
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, primes.size() - 1);

    BigNum p = BigNum("503");
    BigNum q=BigNum("509");
   /* do {
        q = BigNum(primes[distrib(gen)]);
    } while (p == q);*/

    n = p * q;
    std::cout<<"n=";
    n.print();
    std::cout<<"\n";
    BigNum phi = (p - 1) * (q - 1);
    std::cout<<"phi=";
    phi.print();
    std::cout<<"\n";
    e = 3;
    while (BigNum::Nod(e, phi) != 1) {
        e = e + BigNum(1);
    }
    std::cout<<"e=";
    e.print();
    std::cout<<"\n";
    BigNum k(1);
    while ((BigNum(1) + k * phi) % e != 0) {
        k = k + BigNum(1);
        std::cout<<"k=";
        k.print();
        std::cout<<"\n";
    }
    d = (BigNum(1) + k * phi) / e;
    std::cout<<"d=";
    d.print();
    std::cout<<"\n";
}

std::vector<BigNum> Rsa(const std::string& mes, BigNum e, BigNum n) {
    std::vector<BigNum> MesASCII;
    for (char c : mes) {
        BigNum num(c); 
        BigNum numm=BigNum::PowNum(num, e, n);
        std::cout<<"Num cod:";
        numm.print();
        std::cout<<std::endl;
        MesASCII.push_back(numm);
    }
    return MesASCII;
}

std::string DeRsa(const std::vector<BigNum>& mes, BigNum d, BigNum n) {
    std::string MesASCII;
    for (const BigNum& c : mes) {
        BigNum num = BigNum::PowNum(c, d, n);
        std::cout<<"Num Decod:";
        num.print();
        std::cout<<std::endl;
       MesASCII += num.toChar();
    }
    return MesASCII;
}

int main() {
    BigNum e, d, n;
    generateKey(e, d, n);

    std::cout << "Public exponent (e): ";
    e.print();
    std::cout<<"\n";
    std::cout << "Private exponent (d): ";
    d.print();
     std::cout<<"\n";
    std::cout << "Modulus (n): ";
    n.print();
    std::cout<<"\n";

    std::string word = "SDFSDFSDF";
    std::vector<BigNum> RsaCode = Rsa(word, e, n);
    
    std::cout << "Encrypted message: ";
    std::cout<<"\n";
    for (const auto& i : RsaCode) {
        i.print();
        std::cout<<" ";
    }
     std::cout<<"\n";
    //Дешифрование
    std::string decryptedMessage = DeRsa(RsaCode, d, n);
    std::cout << "Decrypted message: " << decryptedMessage << std::endl;

    return 0;
}
