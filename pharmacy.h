#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class Hospital;

class Pharmacy {
private:
  string pharmacyID;
  string name;
  string location; // Physical address of the pharmacy
  map<string, double> hospitalBills; // Track bills for each hospital
  vector<string> availableMedications; // List of medications this pharmacy can provide

public:
  Pharmacy(string id, string n);
  Pharmacy(string id, string n, string loc);
  
  // Medication and billing methods
  bool deliverMedication(Hospital* hospital, const string& patientID, const string& medication, double cost);
  double getBillForHospital(const string& hospitalID) const;
  void receiveBillPayment(const string& hospitalID, double amount);
  bool hasMedication(const string& medication) const;
  void addMedication(const string& medication);
  
  // Getters
  string getPharmacyID() const;
  string getName() const;
  string getLocation() const;
  string getStatusReport() const;
  map<string, double> getAllBills() const;
  
  // Setters
  void setLocation(const string& loc);
};

#endif // PHARMACY_H
