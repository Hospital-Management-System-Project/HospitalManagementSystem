#ifndef DRUG_H
#define DRUG_H

#include <string>

class drug {
public:
    std::string drugName;
    double price;

    drug(const std::string& name, double p){
        drugName = name;
        price = p;
    }
};

#endif // DRUG_H