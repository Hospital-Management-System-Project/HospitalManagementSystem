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

Nurse* Hospital::findNurse(string nurseID) {
    for (auto nurse : nurses) {
        if (nurse->nurseID == nurseID) {
            return nurse;
        }
    }
    return nullptr;
}

bool Hospital::assignNurseToPatient(string nurseID, string patientID) {
    // Find the nurse
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return false; // Nurse not found
    }
    
    // Find the patient
    Patient* patient = nullptr;
    for (auto p : patients) {
        if (p->patientID == patientID) {
            patient = p;
            break;
        }
    }
    
    if (!patient) {
        return false; // Patient not found
    }
    
    // Assign nurse to patient (will only succeed if nurse has < 2 patients)
    return nurse->assignPatient(patientID);
}

string Hospital::orderPrescription(string pharmacyID, string patientID, string medication, double price) {
    // Check if patient exists in this hospital
    bool patientFound = false;
    for (auto patient : patients) {
        if (patient->patientID == patientID) {
            patientFound = true;
            break;
        }
    }
    
    if (!patientFound) {
        return ""; // Patient not found in this hospital
    }
    
    // Generate a unique prescription ID
    string prescriptionID = "CX-" + hospitalID + "-" + patientID + "-" + to_string(rand() % 1000);
    
    // Add prescription to pharmacy
    PharmacySystem* pharmacySystem = PharmacySystem::getInstance();
    if (pharmacySystem->addPrescription(pharmacyID, prescriptionID, patientID, medication, price)) {
        // Simulate delivery
        pharmacySystem->deliverMedication(pharmacyID, prescriptionID);
        
        // Create bill
        string billID = pharmacySystem->createBill(pharmacyID, hospitalID, prescriptionID);
        if (!billID.empty()) {
            // Add to hospital's bill tracking
            pharmacyBills[billID] = price;
            return prescriptionID;
        }
    }
    return "";
}

bool Hospital::payPharmacyBill(string pharmacyID, string billID) {
    // Check if bill exists in our records
    auto it = pharmacyBills.find(billID);
    if (it == pharmacyBills.end()) {
        return false; // Bill not found
    }
    
    // Pay the bill
    PharmacySystem* pharmacySystem = PharmacySystem::getInstance();
    if (pharmacySystem->payBill(pharmacyID, billID)) {
        // Mark as paid in our system by removing it
        pharmacyBills.erase(billID);
        return true;
    }
    return false;
}

double Hospital::getTotalPharmacyBills() const {
    double total = 0.0;
    for (const auto& bill : pharmacyBills) {
        total += bill.second;
    }
    return total;
}

double Hospital::getPaidPharmacyBills() const {
    // This is 0 since we remove bills from the map once paid
    return 0.0;
}

double Hospital::getPendingPharmacyBills() const {
    // Same as total since we only track unpaid bills
    return getTotalPharmacyBills();
}

void Hospital::updatePatientDays() {
    // Increment days for all admitted patients
    for (auto patient : patients) {
        if (!patient->discharged) {
            patient->incrementDaysAdmitted();
        }
    }
}

double Hospital::calculatePatientBill(string patientID) const {
    for (auto patient : patients) {
        if (patient->patientID == patientID) {
            return patient->calculateCurrentBill();
        }
    }
    return 0.0; // Patient not found
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
            if (amount <= bill) {
                collectedPayments += amount;
                return true;
            }
            return false; // Payment exceeds bill
        }
    }
    return false; // Patient not found
}

bool Hospital::assignDoctorToPatient(string doctorID, string patientID) {
    // Find the doctor
    Doctor* doctor = nullptr;
    for (auto doc : doctors) {
        if (doc->doctorID == doctorID) {
            doctor = doc;
            break;
        }
    }
    
    if (!doctor) {
        return false; // Doctor not found
    }
    
    // Find the patient
    Patient* patient = nullptr;
    for (auto p : patients) {
        if (p->patientID == patientID) {
            patient = p;
            break;
        }
    }
    
    if (!patient) {
        return false; // Patient not found
    }
    
    // Assign doctor to patient
    doctor->assignPatient(patientID);
    patient->addAttendingDoctor(doctorID);
    
    return true;
}

bool Hospital::requestPatientDischarge(string doctorID, string patientID) {
    // Find the doctor
    Doctor* doctor = nullptr;
    for (auto doc : doctors) {
        if (doc->doctorID == doctorID) {
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
    
    report << "===== " << name << " Patient Billing Report =====\n\n";
    report << left << setw(15) << "Patient ID" << setw(25) << "Name" 
           << setw(10) << "Days" << setw(15) << "Amount Due" << setw(15) << "Status" << "\n";
    report << string(80, '-') << "\n";
    
    for (auto patient : patients) {
        report << left << setw(15) << patient->patientID 
               << setw(25) << patient->name 
               << setw(10) << patient->daysAdmitted 
               << "$" << setw(14) << fixed << setprecision(2) << patient->calculateCurrentBill()
               << setw(15) << patient->getStatus() << "\n";
    }
    
    report << "\n";
    report << "Total Billing: $" << fixed << setprecision(2) << getTotalPatientBills() << "\n";
    report << "Collected Payments: $" << fixed << setprecision(2) << collectedPayments << "\n";
    
    return report.str();
}