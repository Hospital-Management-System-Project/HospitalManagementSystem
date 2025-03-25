#ifndef DRUG_H
#define DRUG_H

#include <string>

class Drug {
public:
    std::string drugName;
    double price;

    Drug(const std::string& name, double p)
        : drugName(name), price(p) {}
};

#endif // DRUG_H
