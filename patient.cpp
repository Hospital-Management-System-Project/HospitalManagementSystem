#include "patient.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// Initialize static member
int Patient::nextPatientNumber = 1000;

// Static method to generate unique IDs
string Patient::generateUniqueID() {
    nextPatientNumber++;
    return "P" + to_string(nextPatientNumber);
}

Patient::Patient(string id, string n, string phone, string d, string t, string docID) {
    patientID = id;
    name = n;
    phoneNumber = phone;
    disease = d;
    treatment = t;
    primaryDoctorID = docID;
    hospitalID = "";
    daysAdmitted = 0;
    isReadyForDischarge = false;
    address = "No address provided";
    emergencyContact = "No emergency contact";
    dateOfBirth = "Unknown";
    admissionDate = time(nullptr); // Current time
}

Patient::Patient(string n, string phone, string addr, string emergency, string dob, 
                 string d, string t, string docID) {
    patientID = generateUniqueID(); // Auto-generate ID
    name = n;
    phoneNumber = phone;
    address = addr;
    emergencyContact = emergency;
    dateOfBirth = dob;
    disease = d;
    treatment = t;
    primaryDoctorID = docID;
    hospitalID = "";
    daysAdmitted = 0;
    isReadyForDischarge = false;
    admissionDate = time(nullptr); // Current time
}

void Patient::setPrimaryDoctorID(const string& doctorID) {
    primaryDoctorID = doctorID;
}

void Patient::addAttendingDoctor(const string& doctorID) {
    if (!hasAttendingDoctor(doctorID)) {
        attendingDoctorIDs.push_back(doctorID);
    }
}

void Patient::removeAttendingDoctor(const string& doctorID) {
    auto it = find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), doctorID);
    if (it != attendingDoctorIDs.end()) {
        attendingDoctorIDs.erase(it);
    }
}

bool Patient::hasAttendingDoctor(const string& doctorID) const {
    return find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), doctorID) != attendingDoctorIDs.end();
}

void Patient::addNurse(const string& nurseID) {
    if (!hasNurse(nurseID)) {
        nurseIDs.push_back(nurseID);
    }
}

void Patient::removeNurse(const string& nurseID) {
    auto it = find(nurseIDs.begin(), nurseIDs.end(), nurseID);
    if (it != nurseIDs.end()) {
        nurseIDs.erase(it);
    }
}

bool Patient::hasNurse(const string& nurseID) const {
    return find(nurseIDs.begin(), nurseIDs.end(), nurseID) != nurseIDs.end();
}

void Patient::addMedication(const string& medication) {
    medications.push_back(medication);
}

void Patient::incrementDaysAdmitted() {
    daysAdmitted++;
}

string Patient::getPatientID() const {
    return patientID;
}

string Patient::getName() const {
    return name;
}

string Patient::getPhoneNumber() const {
    return phoneNumber;
}

string Patient::getDisease() const {
    return disease;
}

string Patient::getTreatment() const {
    return treatment;
}

string Patient::getHospitalID() const {
    return hospitalID;
}

string Patient::getPrimaryDoctorID() const {
    return primaryDoctorID;
}

vector<string> Patient::getAttendingDoctors() const {
    return attendingDoctorIDs;
}

vector<string> Patient::getNurses() const {
    return nurseIDs;
}

vector<string> Patient::getMedications() const {
    return medications;
}

int Patient::getDaysAdmitted() const {
    return daysAdmitted;
}

void Patient::setHospitalID(const string& hospID) {
    hospitalID = hospID;
}

void Patient::markReadyForDischarge(bool ready) {
    isReadyForDischarge = ready;
}

bool Patient::getReadyForDischarge() const {
    return isReadyForDischarge;
}

string Patient::getAddress() const {
    return address;
}

string Patient::getEmergencyContact() const {
    return emergencyContact;
}

string Patient::getDateOfBirth() const {
    return dateOfBirth;
}

time_t Patient::getAdmissionDate() const {
    return admissionDate;
}

void Patient::setAddress(const string& addr) {
    address = addr;
}

void Patient::setEmergencyContact(const string& contact) {
    emergencyContact = contact;
}

void Patient::setDateOfBirth(const string& dob) {
    dateOfBirth = dob;
}

double Patient::calculateBill(double dailyRate) const {
    return daysAdmitted * dailyRate;
}

string Patient::getFullDetails() const {
    stringstream details;
    
    details << "PATIENT DETAILS" << endl;
    details << "==============" << endl;
    details << "Patient ID: " << patientID << endl;
    details << "Name: " << name << endl;
    details << "Phone: " << phoneNumber << endl;
    details << "Address: " << address << endl;
    details << "Emergency Contact: " << emergencyContact << endl;
    details << "Date of Birth: " << dateOfBirth << endl;
    details << "Hospital ID: " << hospitalID << endl;
    details << "Disease: " << disease << endl;
    details << "Treatment: " << treatment << endl;
    details << "Days Admitted: " << daysAdmitted << endl;
    
    char* time_str = ctime(&admissionDate);
    details << "Admission Date: " << (time_str ? time_str : "Unknown") << endl;
    
    // Primary doctor
    details << "Primary Doctor: " << primaryDoctorID << endl;
    
    // Attending doctors with count
    details << "Attending Doctors (" << attendingDoctorIDs.size() << "): ";
    if (attendingDoctorIDs.empty()) {
        details << "None" << endl;
    } else {
        details << endl;
        for (const auto& docID : attendingDoctorIDs) {
            details << "  - " << docID << endl;
        }
    }
    
    // Nurses
    details << "Assigned Nurses: ";
    if (nurseIDs.empty()) {
        details << "None" << endl;
    } else {
        for (size_t i = 0; i < nurseIDs.size(); ++i) {
            if (i > 0) details << ", ";
            details << nurseIDs[i];
        }
        details << endl;
    }
    
    // Medications
    details << "Medications: ";
    if (medications.empty()) {
        details << "None" << endl;
    } else {
        for (size_t i = 0; i < medications.size(); ++i) {
            if (i > 0) details << ", ";
            details << medications[i];
        }
        details << endl;
    }
    
    // Discharge status
    details << "Ready for Discharge: " << (isReadyForDischarge ? "Yes" : "No") << endl;
    
    return details.str();
}