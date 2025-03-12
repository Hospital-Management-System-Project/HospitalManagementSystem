#include "hospital.h"
#include <algorithm>
#include <sstream>
#include <iostream>  // Add this include for cout
#include "mainwindow.h" // Include mainwindow.h instead of forward declaring

using namespace std;

Hospital::Hospital(string id, string n) {
  hospitalID = id;
  name = n;
  location = "Main Campus"; // Default location
}

Hospital::Hospital(string id, string n, string loc) {
  hospitalID = id;
  name = n;
  location = loc;
}

bool Hospital::admitPatient(Patient* patient) {
  if (patients.size() < MAX_CAPACITY) {
    patients.push_back(patient);
    patient->setHospitalID(hospitalID);
    return true;
  }
  return false;
}

bool Hospital::relocatePatient(Patient* patient, Hospital* newHospital) {
  // First check if the patient is in this hospital
  if (!hasPatient(patient)) {
    return false;
  }
  
  if (newHospital->admitPatient(patient)) {
    patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
    
    // Update the patient's hospital ID
    patient->setHospitalID(newHospital->hospitalID);
    
    return true;
  }
  return false;
}

// Check if patient is in this hospital
bool Hospital::hasPatient(Patient* patient) const {
  return find(patients.begin(), patients.end(), patient) != patients.end();
}

bool Hospital::dischargePatient(Patient* patient, Doctor* authorizedBy) {
  // Check if discharge is authorized
  // If a doctor is provided, they must be the patient's primary doctor
  // Otherwise, the patient must have been previously marked as ready for discharge
  if (authorizedBy) {
    if (!authorizedBy->isPrimaryPatient(patient->getPatientID())) {
      return false; // Not authorized to discharge
    }
  } else if (!patient->getReadyForDischarge()) {
    return false; // Not authorized for discharge
  }
  
  // First remove patient from any assigned nurses
  for (auto& nurse : nurses) {
    nurse->removePatient(patient->getPatientID());
  }
  
  // Remove patient from any assigned doctors
  for (auto& doctor : doctors) {
    doctor->removePrimaryPatient(patient->getPatientID());
    doctor->removeSecondaryPatient(patient->getPatientID());
  }
  
  // Remove patient from the hospital
  patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
  return true;
}

bool Hospital::authorizePatientDischarge(Doctor* doctor, Patient* patient) {
  if (!doctor->isPrimaryPatient(patient->getPatientID())) {
    return false; // Only primary doctor can authorize
  }
  
  patient->markReadyForDischarge(true);
  return true;
}

double Hospital::calculatePatientBill(Patient* patient) const {
  return patient->calculateBill(DAILY_RATE);
}

void Hospital::addDoctor(Doctor* doctor) {
  doctors.push_back(doctor);
  doctor->setHospitalID(hospitalID);
}

void Hospital::addNurse(Nurse* nurse) {
  nurses.push_back(nurse);
  nurse->setHospitalID(hospitalID);
}

bool Hospital::assignDoctorToPatient(Doctor* doctor, Patient* patient, bool isPrimary) {
  // Check if doctor belongs to this hospital
  if (doctor->getHospitalID() != hospitalID) {
    return false;
  }
  
  // Assign doctor based on primary/secondary status
  if (isPrimary) {
    if (doctor->addPrimaryPatient(patient->getPatientID())) {
      patient->setPrimaryDoctorID(doctor->getDoctorID());
      return true;
    }
  } else {
    if (doctor->addSecondaryPatient(patient->getPatientID())) {
      patient->addAttendingDoctor(doctor->getDoctorID());
      return true;
    }
  }
  return false;
}

bool Hospital::assignNurseToPatient(Nurse* nurse, Patient* patient) {
  // Check if nurse belongs to this hospital
  if (nurse->getHospitalID() != hospitalID) {
    return false;
  }
  
  // Ensure nurse can accept more patients (maximum of 2)
  if (!nurse->canAcceptMorePatients()) {
    return false;
  }
  
  if (nurse->assignPatient(patient->getPatientID())) {
    patient->addNurse(nurse->getNurseID());
    return true;
  }
  return false;
}

bool Hospital::requestMedication(Pharmacy* pharmacy, Patient* patient, const string& medication, double cost) {
  // Record the bill from the pharmacy
  pharmacyBills[pharmacy->getPharmacyID()] += cost;
  
  // Record medication for the patient
  patient->addMedication(medication);
  
  // Tell the pharmacy to deliver and bill the hospital
  pharmacy->deliverMedication(this, patient->getPatientID(), medication, cost);
  
  return true;
}

void Hospital::payPharmacyBill(const string& pharmacyID) {
  // In a real system, this would handle the actual payment
  if (pharmacyBills.find(pharmacyID) != pharmacyBills.end()) {
    double amount = pharmacyBills[pharmacyID];
    pharmacyBills[pharmacyID] = 0.0; // Reset bill after payment
    
    // Notify the pharmacy of the payment
    for (auto& pharmacy : MainWindow::getPharmacies()) {
      if (pharmacy->getPharmacyID() == pharmacyID) {
        pharmacy->receiveBillPayment(hospitalID, amount);
        break;
      }
    }
  }
}

int Hospital::getCurrentCapacity() const {
  return patients.size();
}

bool Hospital::isFull() const {
  return patients.size() >= MAX_CAPACITY;
}

string Hospital::getStatusReport() const {
  stringstream report;
  report << "Hospital ID: " << hospitalID << "\n";
  report << "Name: " << name << "\n";
  report << "Location: " << location << "\n";
  report << "Patients: " << patients.size() << "/" << MAX_CAPACITY << "\n";
  report << "Doctors: " << doctors.size() << "\n";
  report << "Nurses: " << nurses.size() << "\n";
  
  // Add pharmacy bill information
  report << "Pharmacy Bills:\n";
  for (const auto& bill : pharmacyBills) {
    report << "  Pharmacy " << bill.first << ": $" << bill.second << "\n";
  }
  
  return report.str();
}

void Hospital::updatePatientStay() {
  for (auto& patient : patients) {
    patient->incrementDaysAdmitted();
  }
}

int Hospital::getCapacity() const {
  return MAX_CAPACITY;
}

// Method to get total system capacity
int Hospital::getTotalCapacity() {
  return TOTAL_CAPACITY;
}

double Hospital::getDailyRate() {
  return DAILY_RATE;
}

void Hospital::displayStaffingSummary() const {
  stringstream summary;
  summary << "Hospital: " << name << " (ID: " << hospitalID << ")" << endl;
  summary << "Location: " << location << endl;
  summary << "STAFF SUMMARY:" << endl;
  
  // Doctor summary
  summary << "Doctors: " << doctors.size() << endl;
  map<string, int> doctorSpecialtyCounts;
  for (const auto& doctor : doctors) {
    doctorSpecialtyCounts[doctor->getSpecialization()]++;
  }
  
  summary << "Doctor Specialties:" << endl;
  for (const auto& specialty : doctorSpecialtyCounts) {
    summary << "  - " << specialty.first << ": " << specialty.second << endl;
  }
  
  // Nurse summary
  summary << "Nurses: " << nurses.size() << endl;
  map<string, int> nurseSpecialtyCounts;
  for (const auto& nurse : nurses) {
    nurseSpecialtyCounts[nurse->getSpecialization()]++;
  }
  
  summary << "Nurse Specialties:" << endl;
  for (const auto& specialty : nurseSpecialtyCounts) {
    summary << "  - " << specialty.first << ": " << specialty.second << endl;
  }
  
  // Calculate currently assigned patients for nurses
  int nurseAssignments = 0;
  for (const auto& nurse : nurses) {
    nurseAssignments += nurse->getPatientCount();
  }
  
  summary << "Nurse Patient Assignments: " << nurseAssignments << "/" << (nurses.size() * 2) << endl;
  summary << "Nurse Capacity Utilization: " << 
    (nurses.empty() ? 0 : (nurseAssignments * 100.0 / (nurses.size() * 2))) << "%" << endl;
  
  // Don't return a value since the function return type is void
  // Instead, print the summary to console or use it in some other way
  std::cout << summary.str();  // Add std:: namespace qualifier
}
