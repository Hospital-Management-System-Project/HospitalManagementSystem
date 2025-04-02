#ifndef DRUG_H
#define DRUG_H

#include <string>

// Drug class to represent a drug with its name and price
class drug {
// Encapsulated data members
private:
    std::string drugName;
    double price;

// Public member functions
public:
    drug(const std::string& name, double p){
        drugName = name;
        price = p;
    }
    // Getters
    std::string getDrugName() const {
        return drugName;
    }
    double getPrice() const {
        return price;
    }
};
#endif // DRUG_H