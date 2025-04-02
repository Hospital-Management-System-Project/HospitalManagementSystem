#include "patient.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>

using namespace std;

// Constructor for the Patient class to set initial values
Patient::Patient(string id, string name, string phone, string patientDisease, string treatmentType, string docID, string nurseID){
    patientID = id;
    patientName = name;
    phoneNumber = phone;
    disease = patientDisease;
    treatment = treatmentType;
    daysAdmitted = 0;
    primaryDoctorID = docID;
    attendingNursesIDs.push_back(nurseID);
    discharged = false;
    dischargeRequested = (false);
    billingRatePerDay = 100.0; 
    admissionDate = time(nullptr); 
}

// Add an attending doctor to the patient's list of doctors
void Patient::addAttendingDoctor(string docID) {
    if (docID == primaryDoctorID) {
        return;
    }
    // Check if doctor already exists in the list
    if (find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), docID) == attendingDoctorIDs.end()) {
        attendingDoctorIDs.push_back(docID);    // This will push back the doctor ID to adding doctor IDs
    }
}

// Add an attending nurse to the patient's list of nurses
bool Patient::addAttendingNurse(string nurseID) {
    // Check if nurse is already in the list
    if (std::find(attendingNursesIDs.begin(), attendingNursesIDs.end(), nurseID) != attendingNursesIDs.end()) {
        return false;
    }
    // We will push back the nurse ID to adding nurse IDs
    attendingNursesIDs.push_back(nurseID);
    return true;
}

// Keep the debug output in applyRateIncrease but still use normal calculation
void Patient::applyRateIncrease(double percentage) {
    // Store old rate for comparison
    double oldRate = billingRatePerDay;
    // Apply the increase
    billingRatePerDay *= (1.0 + percentage);
    // Round to nearest cent
    billingRatePerDay = round(billingRatePerDay * 100) / 100;
    cout << "Rate increased for patient " << patientID << " from $" << oldRate << " to $" << billingRatePerDay << endl;
}

// Increment the number of days the patient has been admitted
void Patient::incrementDaysAdmitted() {
    daysAdmitted++;    
    // Apply a rate increase every 3 days
    if (daysAdmitted % 3 == 0 && daysAdmitted > 0) {
        cout << "Applying rate increase for patient " << patientID << endl;
        // Our group just pick a random percentage for the sake of example
        applyRateIncrease(0.03);
    }
}

// We are calculating the current bill based on the number of days admitted
double Patient::calculateCurrentBill() const {
    // Always charge at least one day's rate (even for newly admitted patients)
    return billingRatePerDay * max(1, daysAdmitted);
}

// Set the daily rate for the patient
void Patient::setDailyRate(double rate) {
    if (rate > 0) {
        billingRatePerDay = rate;   // This will set the daily rate for the patient
    }
}

// Check if the patient can be discharged by a specific doctor
bool Patient::canBeDischargedBy(string doctorID) const {
    // Primary doctor can always discharge
    if (doctorID == primaryDoctorID) {
        return true;
    }
    // We also allow the attending doctors to discharge
    return find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), doctorID) != attendingDoctorIDs.end();
}

// Get the full description of the patient, including personal, medical, and billing information etc.
string Patient::getFullDescription() const {
    stringstream description;
    // Personal details section
    description << "=== PATIENT DETAILS ===\n";
    description << "Patient ID: " << patientID << "\n";
    description << "Name: " << patientName << "\n";
    description << "Phone Number: " << phoneNumber << "\n";
    // Medical information
    description << "\n=== MEDICAL INFORMATION ===\n";
    description << "Disease: " << disease << "\n";
    description << "Treatment Plan: " << treatment << "\n";
    description << "Status: " << getStatus() << "\n";
    description << "Days Admitted: " << daysAdmitted << "\n";
    description << "Admission Date: " << getAdmissionDateString() << "\n";
    // Billing information
    description << "\n=== BILLING INFORMATION ===\n";
    description << "Daily Rate: $" << fixed << setprecision(2) << billingRatePerDay << "\n";
    description << "Current Bill: $" << fixed << setprecision(2) << calculateCurrentBill() << "\n";
    // Care team
    description << "\n=== CARE TEAM ===\n";
    description << "Primary Doctor: " << primaryDoctorID << "\n";
    
    // Check if there are any attending doctors
    if (!attendingDoctorIDs.empty()) {
        description << "Attending Doctors: ";
        for (size_t i = 0; i < attendingDoctorIDs.size(); i++) {
            description << attendingDoctorIDs[i];
            if (i < attendingDoctorIDs.size() - 1) {
                description << ", ";
            }
        }
        description << "\n";
    }
    description << "Attending Nurses: ";
    for (size_t i = 0; i < attendingNursesIDs.size(); i++) {
        description << attendingNursesIDs[i];
        if (i < attendingNursesIDs.size() - 1) {
            description << ", ";
        }
    }
    description << "\n";    
    return description.str();
}

// Created a function to get the admission date in a readable format
string Patient::getAdmissionDateString() const {
    char buffer[30];
    struct tm* timeinfo = localtime(&admissionDate);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeinfo);
    return string(buffer);
}

string Patient::getStatus() const {
    return discharged ? "Discharged" : "Admitted";
}

// Getter and setter implementations
string Patient::getPatientID() const {
    return patientID;
}

string Patient::getPatientName() const {
    return patientName;
}

string Patient::getDisease() const {
    return disease;
}

int Patient::getDaysAdmitted() const {
    return daysAdmitted;
}

string Patient::getPrimaryDoctorID() const {
    return primaryDoctorID;
}

void Patient::setPrimaryDoctorID(const string& docID) {
    primaryDoctorID = docID;
}

vector<string>& Patient::getAttendingDoctorIDs() {
    return attendingDoctorIDs;
}

vector<string>& Patient::getAttendingNursesIDs() {
    return attendingNursesIDs;
}

const vector<string>& Patient::getAttendingDoctorIDs() const {
    return attendingDoctorIDs;
}

const vector<string>& Patient::getAttendingNursesIDs() const {
    return attendingNursesIDs;
}

double Patient::getBillingRatePerDay() const {
    return billingRatePerDay;
}

void Patient::setDischarged(bool status) {
    discharged = status;
}

bool Patient::isDischargeRequested() const {
    return dischargeRequested;
}

void Patient::setDischargeRequested(bool status) {
    dischargeRequested = status;
}

string Patient::getPhoneNumber() const {
    return phoneNumber;
}

string Patient::getTreatment() const {
    return treatment;
}