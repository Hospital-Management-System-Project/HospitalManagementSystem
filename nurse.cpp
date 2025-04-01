#include "nurse.h"
#include <algorithm>
#include <sstream>

using namespace std;

Nurse::Nurse(string id, string name, string hospID) {
  nurseID = id;
  nurseName = name;
  hospitalID = hospID;
}

string Nurse::getNurseID() const {
    return nurseID;
}

string Nurse::getNurseName() const {
    return nurseName;
}

string Nurse::getHospitalID() const {
    return hospitalID;
}

vector<string> Nurse::getPatientIDs() const {
    return patientIDs;
}

void Nurse::setHospitalID(string hosID) {
    this->hospitalID = hosID;
}

bool Nurse::assignPatient(string patientID) {
  if (patientIDs.size() < 2) {
    patientIDs.push_back(patientID);
    return true;
  }
  return false;
}

void Nurse::removePatient(string patientID) {
  patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}

string Nurse::getFullDescription() const {
    stringstream description;

    // Nurse info
    description << "=== NURSE DETAILS ===\n";
    description << "Nurse ID: " << getNurseID() << "\n";
    description << "Name: " << getNurseName() << "\n";

    // Assigned patients
    description << "=== NURSE'S PATIENTS ===\n";
    if (!getPatientIDs().empty()) {
        description << "Assigned Patients: ";
        for (size_t i = 0; i < getPatientIDs().size(); i++) {
            description << getPatientIDs()[i];
            if (i < getPatientIDs().size() - 1) {
                description << ", ";
            }
        }
        description << "\n";
    } else {
        description << "No assigned patients at this time.\n";
    }

    return description.str();
}
