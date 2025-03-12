#include "doctor.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "patient.h"

using namespace std;

Doctor::Doctor(string id, string n, string hospID){
    doctorID = id;
    name = n;
    hospitalID = hospID;
    specialization = "General";
    primaryPatients.clear();
    secondaryPatients.clear();
}

Doctor::Doctor(string id, string n, string hospID, string spec){
    doctorID = id;
    name = n;
    hospitalID = hospID;
    specialization = spec;
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

bool Doctor::authorizeDischarge(const string& patientID) {
    // Only primary doctors can authorize discharge
    if (!isPrimaryPatient(patientID)) {
        return false;
    }
    
    // In a real implementation, this would interact with the patient directly
    // For now, we'll assume success once we verified this is the patient's primary doctor
    return true;
}

string Doctor::getSpecialization() const {
    return specialization;
}

void Doctor::setSpecialization(const string& spec) {
    specialization = spec;
}

string Doctor::getDetails() const {
    stringstream details;
    
    details << "DOCTOR DETAILS" << endl;
    details << "==============" << endl;
    details << "Doctor ID: " << doctorID << endl;
    details << "Name: " << name << endl;
    details << "Hospital ID: " << hospitalID << endl;
    details << "Specialization: " << specialization << endl;
    details << "Primary Patients: " << primaryPatients.size() << "/" << MAX_PRIMARY_PATIENTS << endl;
    details << "Secondary Patients: " << secondaryPatients.size() << endl;
    
    details << "Primary Patient IDs:" << endl;
    for (const auto& id : primaryPatients) {
        details << "- " << id << endl;
    }
    
    details << "Secondary Patient IDs:" << endl;
    for (const auto& id : secondaryPatients) {
        details << "- " << id << endl;
    }
    
    return details.str();
}
