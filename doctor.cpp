#include "doctor.h"
#include <algorithm>
#include <sstream>
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

string Doctor::getFullDescription() const {
    stringstream description;

    // doctor info
    description << "=== DOCTOR DETAILS ===\n";
    description << "Doctor ID: " << getDoctorID() << "\n";
    description << "Name: " << getDoctorName() << "\n";

    // assigned patients
    description << "=== DOCTOR'S PATIENTS ===\n";
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
        description << "No assigned patients at this time" << "\n";
    }

    return description.str();
}

vector<string> Doctor::getPatientIDs() const {
    return patientIDs;
}

void Doctor::setHospitalID(string hospID) {
    this->hospitalID = hospID;
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
