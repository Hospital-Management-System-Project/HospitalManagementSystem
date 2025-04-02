#include "pharmacy.h"
#include <algorithm>

using namespace std;

// Define the constructor for the Pharmacy class with id and name
Pharmacy::Pharmacy(string id, string name) {
    pharmacyID = id;
    pharmacyName = name;
    address = "";
    phoneNumber = "";
}

// Define the constructor for the Pharmacy class with id, name, address, and phone
Pharmacy::Pharmacy(string id, string name, string addr, string phone) {
    pharmacyID = id;
    pharmacyName = name;
    address = addr;
    phoneNumber = phone;
}

// Define the getters for the Pharmacy class
string Pharmacy::getPharmacyID() const {
    return pharmacyID;
}
string Pharmacy::getPharmacyName() const {
    return pharmacyName;
}

// Define the function to get the total amount of pending payments
double Pharmacy::getPendingPayments() const {
    double totalAmt = 0.0;
    for (const auto& bill : bills) {    // Loop through all the bills
        if (!bill.paid) {   // Check if the bill is unpaid
            totalAmt += bill.amount;    // Add the amount to the total
        }
    }
    return totalAmt;
}

// Define the function to get all bills for a specific hospital
vector<Bill> Pharmacy::getBillsForHospital(string hospitalID) const {
    vector<Bill> hospitalBills;         // Vector to store bills for the hospital
    for (const auto& bill : bills) {    // Loop through all the bills
        if (bill.hospitalID == hospitalID) {
            hospitalBills.push_back(bill);  // Add the bill to the vector if it belongs to the hospital
        }
    }
    return hospitalBills;
}

// Define the function to bill a hospital for a drug
string Pharmacy::billHospitalForDrug(string hospitalID, string drugName, double price) {
    string formattedDrugName = drugName;
    replace(formattedDrugName.begin(), formattedDrugName.end(), ' ', '_');
    // Generate a unique bill ID
    string billID = "DRUGBILL-" + pharmacyID + "-" + hospitalID + "-" + formattedDrugName + "-" + to_string(rand() % 100000);
    // We will create a new bill and add it to the vector
    bills.push_back(Bill(billID, hospitalID, price));
    return billID;
}