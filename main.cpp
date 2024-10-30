#include <iostream>
#include "Rsa.h"
#include <random>

using std::cin;
using std::cout;

void generateKey(BigNum& e, BigNum& d, BigNum& n) {
    BigNum p = BigNum("11111111111111111111111111111111111111116661111111111111111111111111111111111111111");
    BigNum q=BigNum("14361817905487073108115025320764062442342597502003905253144371337891157457030439301");
    n = p * q;
    // std::cout<<"n=";
    // n.Print();
    // std::cout<<"\n";
    BigNum phi = (p - BigNum("1")) * (q - BigNum("1"));
    // std::cout<<"phi=";
    // phi.Print();
    // std::cout<<"\n";
    e = BigNum("3");
    while (BigNum::Nod(e, phi) != BigNum("1")) {
        e = e + BigNum("1");
    }
    //std::cout<<"e=";
    //e.Print();
    //std::cout<<"\n";
    BigNum k("1");
    while ((BigNum("1")+k*phi) % e != BigNum("0")) {
        k = k + BigNum("1");
        //std::cout<<"k=";
        //k.Print();
        //std::cout<<"\n";
    }
    d = (BigNum("1") + k * phi) / e;
    // std::cout<<"d=";
    // d.Print();
    // std::cout<<"\n";
}

std::vector<BigNum> Rsa(const std::string& mes, BigNum e, BigNum n) {
    std::vector<BigNum> MesASCII;
    for (char c : mes) {
        BigNum num(std::to_string(c)); 
        BigNum numm=BigNum::PowNum(num, e, n);
        // std::cout<<"Num cod:";
        // numm.Print();
        // std::cout<<std::endl;
        MesASCII.push_back(numm);
    }
    return MesASCII;
}

std::string DeRsa(const std::vector<BigNum>& mes, BigNum d, BigNum n) {
    std::string MesASCII;

    for (const BigNum& c : mes) {
        BigNum num = BigNum::PowNum(c, d, n);
        // std::cout << "Num Decod:";
        // num.Print();
        // std::cout << std::endl;
        MesASCII += num.toASCII(); 
    }

    return MesASCII;
}

int main(){
    BigNum e, d, n;
    generateKey(e, d, n);

    std::cout << "Public exponent (e): ";
    e.Print();
    std::cout<<"\n";
    std::cout << "Private exponent (d): ";
    d.Print();
    std::cout<<"\n";
    std::cout << "Modulus (n): ";
    n.Print();
    std::cout<<"\n";

    std::string word = "I HATE THIS COD AND ALGORITHM";
    std::vector<BigNum> RsaCode = Rsa(word, e, n);
    
    std::cout << "Encrypted message: ";
    std::cout<<"\n";
    for (const auto& i : RsaCode) {
        i.Print();
        std::cout<<" ";
    }
    std::cout<<"\n";
    //Дешифрование
    std::string decryptedMessage = DeRsa(RsaCode, d, n);
    std::cout << "Decrypted message: " << decryptedMessage << std::endl;
    return 0;
}