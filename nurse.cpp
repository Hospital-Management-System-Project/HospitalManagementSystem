#include "nurse.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

Nurse::Nurse(string id, string n, string hospID) {
  nurseID = id;
  name = n;
  hospitalID = hospID;
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

string Nurse::getNurseID() const {
  return nurseID;
}

string Nurse::getName() const {
  return name;
}

string Nurse::getHospitalID() const {
  return hospitalID;
}

void Nurse::setHospitalID(const string& hospID) {
  hospitalID = hospID;
}

vector<string> Nurse::getPatients() const {
  return patientIDs;
}
