#include "doctor.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

Doctor::Doctor(string id, string n, string hospID){
    doctorID = id;
    name = n;
    hospitalID = hospID;
    primaryPatients.clear();
    secondaryPatients.clear();
}

bool Doctor::addPrimaryPatient(const string& patientID) {
    // Check if already at maximum capacity for primary patients
    if (primaryPatients.size() >= MAX_PRIMARY_PATIENTS) {
        return false;
    }
    
    // Check if patient is already assigned as primary
    if (isPrimaryPatient(patientID)) {
        return false;
    }
    
    primaryPatients.push_back(patientID);
    return true;
}

bool Doctor::addSecondaryPatient(const string& patientID) {
    // Check if patient is already assigned as secondary
    if (isSecondaryPatient(patientID)) {
        return false;
    }
    
    secondaryPatients.push_back(patientID);
    return true;
}

bool Doctor::removePrimaryPatient(const string& patientID) {
    auto it = find(primaryPatients.begin(), primaryPatients.end(), patientID);
    if (it != primaryPatients.end()) {
        primaryPatients.erase(it);
        return true;
    }
    return false;
}

bool Doctor::removeSecondaryPatient(const string& patientID) {
    auto it = find(secondaryPatients.begin(), secondaryPatients.end(), patientID);
    if (it != secondaryPatients.end()) {
        secondaryPatients.erase(it);
        return true;
    }
    return false;
}

bool Doctor::isPrimaryPatient(const string& patientID) const {
    return find(primaryPatients.begin(), primaryPatients.end(), patientID) != primaryPatients.end();
}

bool Doctor::isSecondaryPatient(const string& patientID) const {
    return find(secondaryPatients.begin(), secondaryPatients.end(), patientID) != secondaryPatients.end();
}

bool Doctor::isPatient(const string& patientID) const {
    return isPrimaryPatient(patientID) || isSecondaryPatient(patientID);
}

void Doctor::setHospitalID(const string& hospID) {
    hospitalID = hospID;
}

string Doctor::getDoctorID() const {
    return doctorID;
}

string Doctor::getName() const {
    return name;
}

string Doctor::getHospitalID() const {
    return hospitalID;
}

vector<string> Doctor::getPrimaryPatients() const {
    return primaryPatients;
}

vector<string> Doctor::getSecondaryPatients() const {
    return secondaryPatients;
}

int Doctor::getPrimaryPatientCount() const {
    return primaryPatients.size();
}

int Doctor::getSecondaryPatientCount() const {
    return secondaryPatients.size();
}
