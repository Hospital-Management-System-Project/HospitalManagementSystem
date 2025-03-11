#include "hospital.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

Hospital::Hospital(std::string id, std::string n){
  hospitalID = id;
  name = n;
}

bool Hospital::admitPatient(Patient *patient) {
  if (patients.size() < 20) {
    patients.push_back(patient);
    return true;
  }
  return false;
}

bool Hospital::relocatePatient(Patient *patient, Hospital *newHospital) {
  if (newHospital->admitPatient(patient)) {
    // Use std::remove and erase to remove the patient from the current hospital
    patients.erase(std::remove(patients.begin(), patients.end(), patient), patients.end());
    return true;
  }
  return false;
}

void Hospital::dischargePatient(Patient *patient) {
  // Use std::remove and erase to remove the patient from the hospital
  patients.erase(std::remove(patients.begin(), patients.end(), patient), patients.end());
}

void Hospital::addDoctor(Doctor *doctor) { 
    doctors.push_back(doctor); 
}

void Hospital::addNurse(Nurse *nurse) { 
    nurses.push_back(nurse); 
}
