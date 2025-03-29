#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Forward declaration to avoid circular dependency
class Hospital;

// Structure to represent a prescription
struct Prescription {
    string prescriptionID;
    string patientID;
    string medication;
    double price;
    bool fulfilled;
    
    Prescription(string id, string patID, string med, double p){
        prescriptionID = id;
        patientID = patID;
        medication = med;
        price = p;
        fulfilled = false;
    } 
};

// Structure to represent a bill
struct Bill {
    string billID;
    string hospitalID;
    double amount;
    bool paid;
    
    Bill(string id, string hospID, double amt){
        billID = id;
        hospitalID = hospID;
        amount = amt;
        paid = false;
    }
};

class Pharmacy {
public:
    string pharmacyID;
    string parmacyName;
    string address;
    string phoneNumber;
    vector<Prescription> prescriptions;
    vector<Bill> bills;
    
    Pharmacy(string id, string n);
    Pharmacy(string id, string n, string addr, string phone);
    
    // Prescription management
    bool receivePrescription(string prescriptionID, string patientID, string medication, double price);
    bool deliverMedication(string prescriptionID);
    
    // Billing functions
    string billHospital(string hospitalID, string prescriptionID);
    string billHospitalForDrug(string hospitalID, string drugName, double price);
    bool receivePayment(string billID);
    
    // Reporting
    double getTotalRevenue() const;
    double getPendingPayments() const;
    vector<Bill> getBillsForHospital(string hospitalID) const;
};

#endif // PHARMACY_H