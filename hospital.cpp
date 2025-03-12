#include "hospital.h"
#include <algorithm>
#include <sstream>

using namespace std;

Hospital::Hospital(string id, string n) {
  hospitalID = id;
  name = n;
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
  if (newHospital->admitPatient(patient)) {
    patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
    
    // Update the patient's hospital ID
    patient->setHospitalID(newHospital->hospitalID);
    
    return true;
  }
  return false;
}

void Hospital::dischargePatient(Patient* patient) {
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
}

double Hospital::calculatePatientBill(Patient* patient) const {
  // Assuming a daily rate of $500 for hospital stay
  const double DAILY_RATE = 500.0;
  return patient->getDaysAdmitted() * DAILY_RATE;
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
  return true;
}

void Hospital::payPharmacyBill(const string& pharmacyID) {
  // In a real system, this would handle the actual payment
  if (pharmacyBills.find(pharmacyID) != pharmacyBills.end()) {
    pharmacyBills[pharmacyID] = 0.0; // Reset bill after payment
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
