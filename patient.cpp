#include "patient.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

Patient::Patient(string id, string name, string phone, string d, string t, string docID){
    patientID = id;
    patientName = name;
    phoneNumber = phone;
    disease = d;
    treatment = t;
    daysAdmitted = 0;
    primaryDoctorID = docID;
    discharged = false;
    billingRatePerDay = 100.0; // Default daily rate
    admissionDate = time(nullptr); // Current time
}

void Patient::addAttendingDoctor(string docID) {
    // Don't add the primary doctor as an attending doctor
    if (docID == primaryDoctorID) {
        return;
    }
    
    // Check if doctor already exists in the list
    if (find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), docID) == attendingDoctorIDs.end()) {
        attendingDoctorIDs.push_back(docID);
    }
}

void Patient::incrementDaysAdmitted() {
    if (!discharged) {
        daysAdmitted++;
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
    
    return description.str();
}

void Patient::markAsDischargedBy(string doctorID) {
    if (canBeDischargedBy(doctorID)) {
        discharged = true;
    }
}

string Patient::getAdmissionDateString() const {
    char buffer[30];
    struct tm* timeinfo = localtime(&admissionDate);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", timeinfo);
    return string(buffer);
}

int Patient::getDaysAdmitted() const {
    return daysAdmitted;
}

string Patient::getStatus() const {
    return discharged ? "Discharged" : "Admitted";
}
