#ifndef DRUG_H
#define DRUG_H

#include <string>

class drug {
private:
    std::string drugName;
    double price;

public:
    drug(const std::string& name, double p) : drugName(name), price(p) {}

    // Getters
    std::string getDrugName() const {
        return drugName;
    }

    double getPrice() const {
        return price;
    }
};

#endif // DRUG_H
