#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <vector>

using namespace std;

// Structure to represent a bill
struct Bill {
    string billID;
    string hospitalID;
    double amount;
    bool paid;

    Bill(string id, string hospID, double amt)
        : billID(id), hospitalID(hospID), amount(amt), paid(false) {}
};

class Pharmacy {
private:
    string pharmacyID;
    string pharmacyName;
    string address;
    string phoneNumber;
    vector<Bill> bills;

public:
    Pharmacy(string id, string n);
    Pharmacy(string id, string n, string addr, string phone);

    // Getters
    string getPharmacyID() const;
    string getPharmacyName() const;

    // Billing functions
    string billHospitalForDrug(string hospitalID, string drugName, double price);

    // Reporting
    double getPendingPayments() const;
    vector<Bill> getBillsForHospital(string hospitalID) const;
};

#endif // PHARMACY_H
