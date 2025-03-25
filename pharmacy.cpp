#include "pharmacy.h"
#include <algorithm>

using namespace std;

Pharmacy::Pharmacy(string id, string n) {
    pharmacyID = id;
    parmacyName = n;
    address = "";
    phoneNumber = "";
}

Pharmacy::Pharmacy(string id, string n, string addr, string phone) {
    pharmacyID = id;
    parmacyName = n;
    address = addr;
    phoneNumber = phone;
}

bool Pharmacy::receivePrescription(string prescriptionID, string patientID, string medication, double price) {
    // Check if prescription already exists
    for (const auto& p : prescriptions) {
        if (p.prescriptionID == prescriptionID) {
            return false; // Prescription ID already exists
        }
    }
    
    // Add new prescription
    prescriptions.push_back(Prescription(prescriptionID, patientID, medication, price));
    return true;
}

bool Pharmacy::deliverMedication(string prescriptionID) {
    for (auto& p : prescriptions) {
        if (p.prescriptionID == prescriptionID && !p.fulfilled) {
            p.fulfilled = true;
            return true;
        }
    }
    return false; // Prescription not found or already fulfilled
}

string Pharmacy::billHospital(string hospitalID, string prescriptionID) {
    // Find the prescription
    for (const auto& p : prescriptions) {
        if (p.prescriptionID == prescriptionID && p.fulfilled) {
            // Create a unique bill ID
            string billID = "Bill" + prescriptionID;
            
            // Create and add the bill
            bills.push_back(Bill(billID, hospitalID, p.price));
            return billID;
        }
    }
    return ""; // Prescription not found or not fulfilled
}

bool Pharmacy::receivePayment(string billID) {
    for (auto& b : bills) {
        if (b.billID == billID && !b.paid) {
            b.paid = true;
            return true;
        }
    }
    return false; // Bill not found or already paid
}

double Pharmacy::getTotalRevenue() const {
    double total = 0.0;
    for (const auto& b : bills) {
        if (b.paid) {
            total += b.amount;
        }
    }
    return total;
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
    // Create a unique bill ID
    string billID = "DRUGBILL-" + pharmacyID + "-" + hospitalID + "-" + to_string(rand() % 100000);

    // Add bill
    bills.push_back(Bill(billID, hospitalID, price));

    return billID;
}
