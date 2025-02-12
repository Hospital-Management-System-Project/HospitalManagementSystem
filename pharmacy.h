#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>

class Pharmacy {
public:
    std::string pharmacyID;
    std::string name;

    Pharmacy(std::string id, std::string n);
};

#endif // PHARMACY_H
