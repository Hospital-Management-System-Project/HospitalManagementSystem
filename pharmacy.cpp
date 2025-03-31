#include "pharmacy.h"
#include <algorithm>

using namespace std;

Pharmacy::Pharmacy(string id, string n) {
    pharmacyID = id;
    pharmacyName = n;
    address = "";
    phoneNumber = "";
}

Pharmacy::Pharmacy(string id, string n, string addr, string phone) {
    pharmacyID = id;
    pharmacyName = n;
    address = addr;
    phoneNumber = phone;
}

string Pharmacy::getPharmacyID() const {
    return pharmacyID;
}

string Pharmacy::getPharmacyName() const {
    return pharmacyName;
}

string Pharmacy::getAddress() const {
    return address;
}

string Pharmacy::getPhoneNumber() const {
    return phoneNumber;
}

const vector<Prescription>& Pharmacy::getPrescriptions() const {
    return prescriptions;
}

const vector<Bill>& Pharmacy::getBills() const {
    return bills;
}

double Pharmacy::getPendingPayments() const {
    double total = 0.0;
    for (const auto& b : bills) {
        if (!b.paid) {
            total += b.amount;
        }
    }
    return total;
}

vector<Bill> Pharmacy::getBillsForHospital(string hospitalID) const {
    vector<Bill> hospitalBills;
    for (const auto& b : bills) {
        if (b.hospitalID == hospitalID) {
            hospitalBills.push_back(b);
        }
    }
    return hospitalBills;
}

string Pharmacy::billHospitalForDrug(string hospitalID, string drugName, double price) {
    string sanitizedDrugName = drugName;
    replace(sanitizedDrugName.begin(), sanitizedDrugName.end(), ' ', '_');
    
    string billID = "DRUGBILL-" + pharmacyID + "-" + hospitalID + "-" + 
                    sanitizedDrugName + "-" + to_string(rand() % 100000);

    // Add bill
    bills.push_back(Bill(billID, hospitalID, price));

    return billID;
}
