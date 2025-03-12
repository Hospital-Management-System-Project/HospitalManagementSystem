#include "nurse.h"
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

Nurse::Nurse(string id, string n, string hospID) {
  nurseID = id;
  name = n;
  hospitalID = hospID;
  specialization = "General";
}

Nurse::Nurse(string id, string n, string hospID, string spec) {
  nurseID = id;
  name = n;
  hospitalID = hospID;
  specialization = spec;
}

bool Nurse::assignPatient(const string& patientID) {
  // Check if already at maximum capacity
  if (patientIDs.size() >= MAX_PATIENTS) {
    return false;
  }
  
  // Check if patient is already assigned
  if (isResponsibleFor(patientID)) {
    return false;
  }
  
  patientIDs.push_back(patientID);
  return true;
}

void Nurse::removePatient(const string& patientID) {
  auto it = find(patientIDs.begin(), patientIDs.end(), patientID);
  if (it != patientIDs.end()) {
    patientIDs.erase(it);
  }
}

bool Nurse::isResponsibleFor(const string& patientID) const {
  return find(patientIDs.begin(), patientIDs.end(), patientID) != patientIDs.end();
}

int Nurse::getPatientCount() const {
  return patientIDs.size();
}

bool Nurse::canAcceptMorePatients() const {
  return patientIDs.size() < MAX_PATIENTS;
}

string Nurse::getNurseID() const {
  return nurseID;
}

string Nurse::getName() const {
  return name;
}

string Nurse::getHospitalID() const {
  return hospitalID;
}

string Nurse::getSpecialization() const {
  return specialization;
}

void Nurse::setHospitalID(const string& hospID) {
  hospitalID = hospID;
}

void Nurse::setSpecialization(const string& spec) {
  specialization = spec;
}

vector<string> Nurse::getPatients() const {
  return patientIDs;
}

string Nurse::getDetails() const {
  stringstream details;
  
  details << "NURSE DETAILS" << endl;
  details << "=============" << endl;
  details << "Nurse ID: " << nurseID << endl;
  details << "Name: " << name << endl;
  details << "Hospital ID: " << hospitalID << endl;
  details << "Specialization: " << specialization << endl;
  details << "Current Patient Load: " << patientIDs.size() << "/" << MAX_PATIENTS << endl;
  
  details << "Assigned Patients:" << endl;
  if (patientIDs.empty()) {
    details << "  None" << endl;
  } else {
    for (const auto& patientID : patientIDs) {
      details << "  - " << patientID << endl;
    }
  }
  
  return details.str();
}
