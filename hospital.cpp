#include "hospital.h"
#include <algorithm> // For std::remove
#include "pharmacysystem.h"
#include <sstream>
#include <iomanip>

using namespace std;

Hospital::Hospital(string id, string hospName) {
    hospitalID = id;
    hospitalName = hospName;
    totalPatientBillings = 0.0;
    collectedPayments = 0.0;
}

bool Hospital::admitPatient(Patient* patient) {
    if (patients.size() < 20) {
        patients.push_back(patient);
        return true;
    }
    return false;
}

bool Hospital::relocatePatient(Patient* patient, Hospital* newHospital) {
    if (newHospital->admitPatient(patient)) {
        // Use remove and erase to remove the patient from the current hospital
        patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
        return true;
    }
    return false;
}

void Hospital::dischargePatient(Patient* patient) {
    // Mark the patient as discharged
    patient->discharged = true;
    
    // Final billing
    totalPatientBillings += patient->calculateCurrentBill();
    
    // Update doctor assignments
    for (auto doctor : doctors) {
        doctor->removePatient(patient->patientID);
    }
    
    // Update nurse assignments
    for (auto nurse : nurses) {
        nurse->removePatient(patient->patientID);
    }
    
    // Remove from patient list
    patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
}

void Hospital::addDoctor(Doctor* doctor) {
    doctors.push_back(doctor);
}

void Hospital::addNurse(Nurse* nurse) {
    nurses.push_back(nurse);
}

void Hospital::updatePatientDays() {
    // Increment days for all admitted patients
    for (auto patient : patients) {
        if (!patient->discharged) {
            patient->incrementDaysAdmitted();
        }
    }
}

double Hospital::getTotalPatientBills() const {
    double total = 0.0;
    for (auto patient : patients) {
        total += patient->calculateCurrentBill();
    }
    return total;
}

bool Hospital::collectPaymentFromPatient(string patientID, double amount) {
    for (auto patient : patients) {
        if (patient->patientID == patientID) {
            double bill = patient->calculateCurrentBill();
            double alreadyPaid = 0.0;
            
            // Get any existing payments
            auto it = patientPayments.find(patientID);
            if (it != patientPayments.end()) {
                alreadyPaid = it->second;
            }
            
            // Calculate remaining balance
            double remainingBalance = bill - alreadyPaid;
            
            // Check if payment exceeds remaining bill
            // Allow paying the exact amount (changed from > to >) 
            if (amount > remainingBalance) {
                return false; // Payment exceeds remaining bill
            }
            
            // Update payment records
            collectedPayments += amount;
            patientPayments[patientID] = alreadyPaid + amount;
            return true;
        }
    }
    return false; // Patient not found
}

double Hospital::getPatientRemainingBalance(string patientID) const {
    for (auto patient : patients) {
        if (patient->patientID == patientID) {
            double bill = patient->calculateCurrentBill();
            
            // Check for payments
            auto it = patientPayments.find(patientID);
            if (it != patientPayments.end()) {
                return bill - it->second;
            }
            
            return bill; // No payments yet
        }
    }
    return 0.0; // Patient not found
}

bool Hospital::requestPatientDischarge(string doctorID, string patientID) {
    // Find the doctor
    Doctor* doctor = nullptr;
    for (auto doc : doctors) {
        if (doc->getDoctorID() == doctorID) {
            doctor = doc;
            break;
        }
    }
    
    if (!doctor) {
        return false; // Doctor not found
    }
    
    return doctor->requestPatientDischarge(patientID);
}

string Hospital::getPatientBillingReport() const {
    stringstream report;
    
    report << "===== " << hospitalName << " Patient Billing Report =====\n\n";
    report << left << setw(15) << "Patient ID" << setw(25) << "Name" 
           << setw(10) << "Days" << setw(15) << "Amount Due" << setw(15) << "Status" << "\n";
    report << string(80, '-') << "\n";
    
    for (auto patient : patients) {
        report << left << setw(15) << patient->patientID 
               << setw(25) << patient->patientName 
               << setw(10) << patient->daysAdmitted 
               << "$" << setw(14) << fixed << setprecision(2) << patient->calculateCurrentBill()
               << setw(15) << patient->getStatus() << "\n";
    }
    
    report << "\n";
    report << "Total Billing: $" << fixed << setprecision(2) << getTotalPatientBills() << "\n";
    
    return report.str();
}
