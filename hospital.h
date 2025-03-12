#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include "pharmacy.h"
#include <string>
#include <vector>
#include <map>

// Forward declaration to resolve circular dependency
class MainWindow;

using namespace std;

class Hospital {
private:
  static const int MAX_CAPACITY = 20;
  static const int TOTAL_LOCATIONS = 5;
  static const int TOTAL_CAPACITY = 100; // Total capacity across all locations
  static constexpr double DAILY_RATE = 500.0; // Daily rate for patient billing in dollars

public:
  string hospitalID;
  string name;
  string location; // Physical location of this hospital branch
  vector<Patient*> patients;
  vector<Doctor*> doctors;
  vector<Nurse*> nurses;
  map<string, double> pharmacyBills; // Track bills from pharmacies

  Hospital(string id, string n);
  Hospital(string id, string n, string loc); // Constructor with location
  
  // Enhanced patient management
  bool admitPatient(Patient* patient);
  bool relocatePatient(Patient* patient, Hospital* newHospital);
  bool dischargePatient(Patient* patient, Doctor* authorizedBy = nullptr);
  double calculatePatientBill(Patient* patient) const;
  bool hasPatient(Patient* patient) const; // Check if patient is in this hospital
  bool authorizePatientDischarge(Doctor* doctor, Patient* patient);
  
  // Staff management
  void addDoctor(Doctor* doctor);
  void addNurse(Nurse* nurse);
  bool assignDoctorToPatient(Doctor* doctor, Patient* patient, bool isPrimary);
  bool assignNurseToPatient(Nurse* nurse, Patient* patient);
  void displayStaffingSummary() const; // New method for staff summary
  
  // Pharmacy interactions
  bool requestMedication(Pharmacy* pharmacy, Patient* patient, const string& medication, double cost);
  void payPharmacyBill(const string& pharmacyID);
  double getTotalPharmacyBills() const; // Get total of all pharmacy bills
  
  // Hospital status
  int getCurrentCapacity() const;
  bool isFull() const;
  string getStatusReport() const;
  void updatePatientStay(); // Increment days for all patients
  
  // Method to get max capacity
  int getCapacity() const;
  
  // Static method to get total system capacity
  static int getTotalCapacity();
  
  // Getter for daily rate
  static double getDailyRate();
};

#endif // HOSPITAL_H
