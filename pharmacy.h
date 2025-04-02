#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <vector>

using namespace std;    // We used standard namespace for convenience

// Created a struct data structure to represent a bill to the pharmacy/patient
struct Bill {
    string billID;
    string hospitalID;
    double amount;
    bool paid;

    // Constructor for Bill
    Bill(string id, string hospID, double amt) {
        billID = id;
        hospitalID = hospID;
        amount = amt;
        paid = false;
    }
};;

// Created a class to represent a pharmacy
class Pharmacy {
private:
    string pharmacyID;
    string pharmacyName;
    string address;
    string phoneNumber;
    vector<Bill> bills; // Created a vector to store bills

public:
    Pharmacy(string id, string name);
    Pharmacy(string id, string name, string addr, string phone);

    // Getters
    string getPharmacyID() const;
    string getPharmacyName() const;

    // Billing functions
    string billHospitalForDrug(string hospitalID, string drugName, double price);

    // This function will be used to grab the total amount of pending payments
    double getPendingPayments() const;
    vector<Bill> getBillsForHospital(string hospitalID) const;  // Created a vector to get bills for a hospital
};

#endif // PHARMACY_H