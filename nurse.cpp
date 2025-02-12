#include "nurse.h"
#include <algorithm> // For std::remove

Nurse::Nurse(std::string id, std::string n, std::string hospID)
    : nurseID(id), name(n), hospitalID(hospID) {}

bool Nurse::assignPatient(std::string patientID) {
    if (patientIDs.size() < 2) {
        patientIDs.push_back(patientID);
        return true;
    }
    return false;
}

void Nurse::removePatient(std::string patientID) {
    patientIDs.erase(std::remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}
