#include "nurse.h"
#include <algorithm>
#include <sstream>

using namespace std;

// We implement the constructor and member functions of the Nurse class here
Nurse::Nurse(string id, string name, string hospID) {
  nurseID = id;
  nurseName = name;
  hospitalID = hospID;
}

// Getter and setter methods for the Nurse class
string Nurse::getNurseID() const {
    return nurseID;
}
string Nurse::getNurseName() const {
    return nurseName;
}
string Nurse::getHospitalID() const {
    return hospitalID;
}
// We have a getter for the hospitalID, it grabs the patientIDs from the vector
vector<string> Nurse::getPatientIDs() const {
    return patientIDs;
}
void Nurse::setHospitalID(string hosID) {
    this->hospitalID = hosID;
}

// We have a function that assigns a patient to the nurse
bool Nurse::assignPatient(string patientID) {
  if (patientIDs.size() < 2) {
    patientIDs.push_back(patientID);
    return true;
  }
  return false;
}

// We have a function that removes a patient from the nurse
void Nurse::removePatient(string patientID) {
  patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}

// This function will get the full description of the nurse
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
        description << "No Assigned Patients At This Time!" << endl;
    }
    return description.str();
}