#include "pharmacy.h"
#include "hospital.h"
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

Pharmacy::Pharmacy(string id, string n) {
  pharmacyID = id;
  name = n;
  location = "Unknown";
}

Pharmacy::Pharmacy(string id, string n, string loc) {
  pharmacyID = id;
  name = n;
  location = loc;
}

bool Pharmacy::deliverMedication(Hospital* hospital, const string& patientID, const string& medication, double cost) {
  // Add to the bill for this hospital
  hospitalBills[hospital->hospitalID] += cost;
  
  // In a real system, more complex delivery logic would be here
  return true;
}

double Pharmacy::getBillForHospital(const string& hospitalID) const {
  auto it = hospitalBills.find(hospitalID);
  if (it != hospitalBills.end()) {
    return it->second;
  }
  return 0.0;
}

void Pharmacy::receiveBillPayment(const string& hospitalID, double amount) {
  auto it = hospitalBills.find(hospitalID);
  if (it != hospitalBills.end()) {
    it->second -= amount;
    if (it->second < 0) {
      it->second = 0; // Avoid negative balances
    }
  }
}

bool Pharmacy::hasMedication(const string& medication) const {
  return find(availableMedications.begin(), availableMedications.end(), medication) != availableMedications.end();
}

void Pharmacy::addMedication(const string& medication) {
  if (!hasMedication(medication)) {
    availableMedications.push_back(medication);
  }
}

string Pharmacy::getPharmacyID() const {
  return pharmacyID;
}

string Pharmacy::getName() const {
  return name;
}

string Pharmacy::getLocation() const {
  return location;
}

void Pharmacy::setLocation(const string& loc) {
  location = loc;
}

string Pharmacy::getStatusReport() const {
  stringstream report;
  report << "Pharmacy ID: " << pharmacyID << "\n";
  report << "Name: " << name << "\n";
  report << "Location: " << location << "\n";
  report << "Outstanding Bills:\n";
  
  double totalBills = 0.0;
  for (const auto& bill : hospitalBills) {
    report << "  Hospital " << bill.first << ": $" << bill.second << "\n";
    totalBills += bill.second;
  }
  
  report << "Total outstanding: $" << totalBills << "\n";
  
  return report.str();
}

map<string, double> Pharmacy::getAllBills() const {
  return hospitalBills;
}
