#include "patient.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>

using namespace std;

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

void Patient::addAttendingDoctor(string docID) {
    if (docID == primaryDoctorID) {
        return;
    }
    
    // Check if doctor already exists in the list
    if (find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), docID) == attendingDoctorIDs.end()) {
        attendingDoctorIDs.push_back(docID);
    }
}

bool Patient::addAttendingNurse(string nurseID) {

    if (std::find(attendingNursesIDs.begin(), attendingNursesIDs.end(), nurseID) != attendingNursesIDs.end()) {
        return false;
    }

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
    
    std::cout << "Rate increased for patient " << patientID << " from $" << oldRate << " to $" << billingRatePerDay << std::endl;
}

void Patient::incrementDaysAdmitted() {
    daysAdmitted++;
    
    //std::cout << "Days admitted for patient " << patientID << " incremented to " << daysAdmitted << std::endl;
    
    // Apply a rate increase every 3 days
    if (daysAdmitted % 3 == 0 && daysAdmitted > 0) {
        std::cout << "Applying rate increase for patient " << patientID << std::endl;
        // Apply a 3% increase
        applyRateIncrease(0.03);
    }
}

double Patient::calculateCurrentBill() const {
    // Always charge at least one day's rate (even for newly admitted patients)
    return billingRatePerDay * max(1, daysAdmitted);
}

void Patient::setDailyRate(double rate) {
    if (rate > 0) {
        billingRatePerDay = rate;
    }
}

bool Patient::canBeDischargedBy(string doctorID) const {
    // Primary doctor can always discharge
    if (doctorID == primaryDoctorID) {
        return true;
    }
    
    // Attending doctors can also discharge
    return find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), doctorID) != attendingDoctorIDs.end();
}

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
