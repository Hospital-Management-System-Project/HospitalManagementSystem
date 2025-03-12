#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include "pharmacy.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class Hospital {
private:
  static const int MAX_CAPACITY = 20;

public:
  string hospitalID;
  string name;
  vector<Patient*> patients;
  vector<Doctor*> doctors;
  vector<Nurse*> nurses;
  map<string, double> pharmacyBills; // Track bills from pharmacies

  Hospital(string id, string n);
  
  // Patient management
  bool admitPatient(Patient* patient);
  bool relocatePatient(Patient* patient, Hospital* newHospital);
  void dischargePatient(Patient* patient);
  double calculatePatientBill(Patient* patient) const;
  
  // Staff management
  void addDoctor(Doctor* doctor);
  void addNurse(Nurse* nurse);
  bool assignDoctorToPatient(Doctor* doctor, Patient* patient, bool isPrimary);
  bool assignNurseToPatient(Nurse* nurse, Patient* patient);
  
  // Pharmacy interactions
  bool requestMedication(Pharmacy* pharmacy, Patient* patient, const string& medication, double cost);
  void payPharmacyBill(const string& pharmacyID);
  
  // Hospital status
  int getCurrentCapacity() const;
  bool isFull() const;
  string getStatusReport() const;
  void updatePatientStay(); // Increment days for all patients
  
  // Method to get max capacity
  int getCapacity() const;
};

#endif // HOSPITAL_H
