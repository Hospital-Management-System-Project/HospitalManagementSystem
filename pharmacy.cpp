#include "pharmacy.h"
#include "hospital.h"
#include <string>

using namespace std;

Pharmacy::Pharmacy(string id, string n) {
  pharmacyID = id;
  name = n;
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

string Pharmacy::getPharmacyID() const {
  return pharmacyID;
}

string Pharmacy::getName() const {
  return name;
}
