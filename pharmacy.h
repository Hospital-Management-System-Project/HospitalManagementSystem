#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <map>

using namespace std;

class Hospital;

class Pharmacy {
private:
  string pharmacyID;
  string name;
  map<string, double> hospitalBills; // Track bills for each hospital

public:
  Pharmacy(string id, string n);
  
  // Medication and billing methods
  bool deliverMedication(Hospital* hospital, const string& patientID, const string& medication, double cost);
  double getBillForHospital(const string& hospitalID) const;
  void receiveBillPayment(const string& hospitalID, double amount);
  
  // Getters
  string getPharmacyID() const;
  string getName() const;
};

#endif // PHARMACY_H
