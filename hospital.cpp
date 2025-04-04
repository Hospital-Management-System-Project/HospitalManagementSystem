#include "hospital.h"
#include <algorithm>
#include "pharmacysystem.h"
#include <sstream>
#include <iomanip>
#include <QDebug>

using namespace std;

// The constructor initializes the hospital ID and name thats passed in
Hospital::Hospital(string id, string hospName) {
    hospitalID = id;
    hospitalName = hospName;
    totalPatientBillings = 0.0;
    collectedPayments = 0.0;
}

// The admitPatient method checks if the hospital has space for a new patient
bool Hospital::admitPatient(Patient* patient) {
    if (patients.size() < 20) { // Requirements say max 20 patients
        patients.push_back(patient);
        return true;
    }
    return false;
}

// The relocatePatient method checks if the new hospital has space for the patient
bool Hospital::relocatePatient(Patient* patient, Hospital* newHospital) {
    if (newHospital->admitPatient(patient)) {
        // Use remove and erase to remove the patient from the current hospital
        patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
        return true;
    }
    return false;
}

// The dischargePatient method marks the patient as discharged and updates the billing
void Hospital::dischargePatient(Patient* patient) {
    patient->setDischarged(true);  // We start by marking the patient as discharged
    totalPatientBillings += patient->calculateCurrentBill(); // Update total billings

    // Next we need to remove the patient from the doctors and nurses
    for (auto doctor : doctors) {
        doctor->removePatient(patient->getPatientID());
    }

    // Need to remove the patient from all nurses
    for (auto nurse : nurses) {
        nurse->removePatient(patient->getPatientID());
    }

    // Remove from patient list
    patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
}

// The removePatient method checks if the patient is in the hospital and removes them
bool Hospital::removePatient(Patient* patient) {
    auto it = find(patients.begin(), patients.end(), patient); // Create an iterator to find the patient
    if (it != patients.end()) {
        patients.erase(it); // Remove the patient from the list
        return true;
    }
    return false;
}

// The addDoctor method adds a doctor to the hospital and sets their hospital ID
void Hospital::addDoctor(Doctor* doctor) {
    doctor->setHospitalID(getHospitalID());
    doctors.push_back(doctor);
}

// The relocateDoctor method checks if the new hospital has space for the doctor
bool Hospital::relocateDoctor(Doctor* doctor, Hospital* newHospital) {
    newHospital->addDoctor(doctor); // Add the doctor to the new hospital
    doctors.erase(remove(doctors.begin(), doctors.end(), doctor), doctors.end()); // Remove from the current hospital
    return true;
}

void Hospital::removeDoctor(Doctor* doctor) {
    // Check if removing this doctor would leave fewer than 3 doctors in the hospital
    if (doctors.size() <= 3) {
        throw std::runtime_error("Cannot Remove Doctor: Hospital Must Have At Least 3 Doctors!");
    }
    // Check if the doctor has any patients before removing
    if (!doctor->getPatientIDs().empty()) {
        throw std::runtime_error("Cannot Remove Doctor With Assigned Patients");
    }
    doctors.erase(remove(doctors.begin(), doctors.end(), doctor), doctors.end());
}

// The addNurse method adds a nurse to the hospital and sets their hospital ID
void Hospital::addNurse(Nurse* nurse) {
    nurse->setHospitalID(getHospitalID());
    nurses.push_back(nurse);
}

void Hospital::removeNurse(Nurse* nurse) {
    // Check if the nurse has any patients before removing
    if (!nurse->getPatientIDs().empty()) {
        throw std::runtime_error("Cannot remove nurse with assigned patients");
    }
    nurses.erase(remove(nurses.begin(), nurses.end(), nurse), nurses.end());
}

// The Getter and Setter methods are used to access the private variables
string Hospital::getHospitalID() const {
    return hospitalID;
}

string Hospital::getHospitalName() const {
    return hospitalName;
}

vector<Patient*> Hospital::getPatients() const {
    return patients;
}

vector<Doctor*> Hospital::getDoctors() const {
    return doctors;
}

vector<Nurse*> Hospital::getNurses()const {
    return nurses;
}

vector<Nurse*>& Hospital::getNurses() {
    return nurses;
}

// The updatePatientDays method increments the days for all admitted patients
void Hospital::updatePatientDays() {
    // Increment days for all admitted patients
    for (auto patient : patients) {
        if (patient->getStatus().compare("Discharged") != 0) {
            patient->incrementDaysAdmitted();
        }
    }
}

// The getTotalPatientBills method calculates the total billings for all patients
double Hospital::getTotalPatientBills() const {
    double total = 0.0;
    for (auto patient : patients) {
        total += patient->calculateCurrentBill(); // Calculate the current bill for each patient
    }
    return total;
}

// The collectPaymentFromPatient method collects payment from a patient
bool Hospital::collectPaymentFromPatient(string patientID, double amount) {
    for (auto patient : patients) {
        if (patient->getPatientID() == patientID) {
            double bill = patient->calculateCurrentBill();
            double alreadyPaid = 0.0;   // Track already paid amount
            // Get any existing payments
            auto it = patientPayments.find(patientID);
            if (it != patientPayments.end()) {
                alreadyPaid = it->second;
            }
            double remainingBalance = bill - alreadyPaid;   // Calculate remaining balance
            // Check if payment exceeds remaining bill
            if (amount > remainingBalance + 0.01) { // Add small tolerance for floating point
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

// The getPatientRemainingBalance method calculates the remaining balance for a patient
double Hospital::getPatientRemainingBalance(string patientID) const {
    for (auto patient : patients) {
        if (patient->getPatientID() == patientID) {
            double bill = patient->calculateCurrentBill();
            // Need to see if there are any payments made
            auto it = patientPayments.find(patientID);
            if (it != patientPayments.end()) {
                double remainingBalance = bill - it->second;
                // We dont want any negative values or small fractional amounts
                return (remainingBalance < 0.01) ? 0.0 : remainingBalance;
            }
            return bill; // No payments yet, return the full bill amount
        }
    }
    return 0.0; // Patient not found
}

// The requestPatientDischarge method checks if the doctor can discharge the patient
bool Hospital::requestPatientDischarge(string doctorID, string patientID) {
    Doctor* doctor = nullptr;   // Pointer to the doctor
    for (auto doc : doctors) {  // Loop through all doctors
        if (doc->getDoctorID() == doctorID) {   // Check if the doctor ID matches
            doctor = doc;   // Set the doctor pointer
            break;
        }
    }
    if (!doctor) {
        return false; // Doctor not found
    }
    return doctor->requestPatientDischarge(patientID);
}

// The getPatientBillingReport method generates a report of all patients and their billing status
string Hospital::getPatientBillingReport() const {
    stringstream report;
    report << "===== " << hospitalName << " Patient Billing Report =====\n\n";
    report << left << setw(15) << "Patient ID" << setw(25) << "Name"
    << setw(10) << "Days" << setw(15) << "Amount Due" << setw(15) << "Status" << "\n";
    report << string(80, '-') << "\n";
    double totalOutstanding = 0.0;
    double totalBilled = 0.0;
    double totalPaid = 0.0;
    
    // Loop through all patients and generate the report
    for (auto patient : patients) {
        // Calculate the current bill for this patient
        double currentBill = patient->calculateCurrentBill();
        totalBilled += currentBill;
        // Check for payments
        double amountPaid = 0.0;
        auto paymentIt = patientPayments.find(patient->getPatientID());
        if (paymentIt != patientPayments.end()) {
            amountPaid = paymentIt->second;
        }
        // Calculate remaining balance
        double remainingBalance = currentBill - amountPaid;
        if (remainingBalance < 0.01) remainingBalance = 0.0; // Avoid tiny amounts
        totalPaid += amountPaid;
        totalOutstanding += remainingBalance;
        
        // Format the report line
        report << left << setw(15) << patient->getPatientID()
               << setw(25) << patient->getPatientName()
               << setw(10) << patient->getDaysAdmitted()
               << "$" << setw(14) << fixed << setprecision(2) << remainingBalance
               << setw(15) << patient->getStatus() << "\n";
    }
    
    report << "\n";
    report << "Total Billed: $" << fixed << setprecision(2) << totalBilled << "\n";
    report << "Total Payments: $" << fixed << setprecision(2) << totalPaid << "\n";
    report << "Outstanding Balance: $" << fixed << setprecision(2) << totalOutstanding << "\n";
    
    return report.str();
}