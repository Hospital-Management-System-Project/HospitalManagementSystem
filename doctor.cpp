#include "doctor.h"
#include <algorithm>
#include "hospitalsystem.h"

using namespace std;

string Doctor::getDoctorID() const {
    return doctorID;
}

string Doctor::getDoctorName() const {
    return doctorName;
}

string Doctor::getHospitalID() const {
    return hospitalID;
}

vector<string> Doctor::getPatientIDs() const {
    return patientIDs;
}

Doctor::Doctor(string id, string n, string hospID) {
    doctorID = id;
    doctorName = n;
    hospitalID = hospID;
}

void Doctor::addPatient(string patientID) {
    // Only add if not already assigned
    if (find(patientIDs.begin(), patientIDs.end(), patientID) == patientIDs.end()) {
        patientIDs.push_back(patientID);
    }
}

void Doctor::removePatient(string patientID) {
    patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}

bool Doctor::requestPatientDischarge(string patientID) {
    // Get the patient from the hospital system
    HospitalSystem* hospitalSystem = HospitalSystem::getInstance();
    Patient* patient = hospitalSystem->findPatient(patientID);
    
    if (!patient) {
        return false; // Patient not found
    }
    
    // Check if doctor can discharge this patient
    if (patient->canBeDischargedBy(doctorID)) {
        patient->setDischargeRequested(true);
        return true;
    }
    
    return false; // Not authorized to discharge
}
