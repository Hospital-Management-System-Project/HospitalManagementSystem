#include "doctor.h"
#include <algorithm>
#include <sstream>
#include "hospitalsystem.h"

using namespace std;

// We defined this constructor to initialize a doctor object
Doctor::Doctor(string id, string n, string hospID) {
    doctorID = id;
    doctorName = n;
    hospitalID = hospID;
}

// Created a method to add a patient to the doctor's list of patients
void Doctor::addPatient(string patientID) {
    // Only add if not already assigned
    if (find(patientIDs.begin(), patientIDs.end(), patientID) == patientIDs.end()) {
        patientIDs.push_back(patientID);
    }
}

// We created a method to remove a patient from the doctor's list of patients
void Doctor::removePatient(string patientID) {
    // This will remove the patientID from the list if it exists
    patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}

// We created this method to request a patient discharge
bool Doctor::requestPatientDischarge(string patientID) {
    // Get the patient from the hospital system
    HospitalSystem* hospitalSystem = HospitalSystem::getInstance();
    Patient* patient = hospitalSystem->findPatient(patientID);
    // Check if patient exists
    if (!patient) {
        return false; // Patient not found
    }
    // Check if doctor can discharge this patient
    if (patient->canBeDischargedBy(doctorID)) {
        patient->setDischargeRequested(true);   // Calls the method to set discharge requested
        return true;
    }
    return false; // Not authorized to discharge
}

// Created the getter and setter methods for the doctor class
string Doctor::getDoctorID() const {
    return doctorID;
}
string Doctor::getDoctorName() const {
    return doctorName;
}
string Doctor::getHospitalID() const {
    return hospitalID;
}
// Created a method to get the full description of the doctor
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
// Created a method to get the patient IDs from the vector
vector<string> Doctor::getPatientIDs() const {
    return patientIDs;
}
// Created a method to set the hospital ID
void Doctor::setHospitalID(string hospID) {
    this->hospitalID = hospID;
}