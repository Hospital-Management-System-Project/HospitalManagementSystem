#include "hospitalsystem.h"
#include <algorithm>
#include <sstream>
#include "pharmacysystem.h"

using namespace std;

HospitalSystem* HospitalSystem::instance = nullptr;

void HospitalSystem::initializeHospitals() {
    const string HOSPITAL_NAMES[5] = {
        "Henry Ford Hospital",
        "Windsor Regional Hospital",
        "Amherstburg Hospital",
        "Tecumseh Hospital",
        "Chatham-Kent Health Alliance"
    };

    // Clear existing hospitals if any
    for (auto hospital : hospitals) {
        delete hospital;
    }
    hospitals.clear();

    // Create the five hospital locations
    for (int i = 0; i < 5; i++) {
        hospitals.push_back(new Hospital(to_string(i + 1), HOSPITAL_NAMES[i]));
    }
    
    // Initialize doctors after hospitals are created
    initializeDoctors();
    
    // Initialize nurses after hospitals and doctors are created
    initializeNurses();
}

void HospitalSystem::initializeDoctors() {
    // Clear existing doctors
    for (auto& pair : doctors) {
        delete pair.second;
    }
    doctors.clear();
    
    // Create at least 3 doctors per hospital (15 total)
    const string DOCTOR_NAMES[15] = {
        "Dr. Smith", "Dr. Johnson", "Dr. Williams",
        "Dr. Brown", "Dr. Jones", "Dr. Miller",
        "Dr. Davis", "Dr. Garcia", "Dr. Rodriguez",
        "Dr. Wilson", "Dr. Martinez", "Dr. Anderson",
        "Dr. Taylor", "Dr. Thomas", "Dr. Hernandez"
    };
    
    int doctorIndex = 0;
    for (size_t i = 0; i < hospitals.size(); i++) {
        Hospital* hospital = hospitals[i];
        
        // Add 3 doctors to each hospital
        for (int j = 0; j < 3; j++) {
            string doctorID = "D" + to_string(doctorIndex + 1);
            Doctor* doctor = new Doctor(doctorID, DOCTOR_NAMES[doctorIndex], hospital->hospitalID);
            
            // Add doctor to system and hospital
            doctors[doctorID] = doctor;
            hospital->addDoctor(doctor);
            
            doctorIndex++;
        }
    }
}

void HospitalSystem::initializeNurses() {
    // Clear existing nurses
    for (auto& pair : nurses) {
        delete pair.second;
    }
    nurses.clear();
    
    // Create at least 5 nurses per hospital (25 total for 5 hospitals)
    const string NURSE_NAMES[25] = {
        "Nurse Adams", "Nurse Baker", "Nurse Clark", "Nurse Davis", "Nurse Evans",
        "Nurse Foster", "Nurse Green", "Nurse Harris", "Nurse Cam", "Nurse Jones",
        "Nurse Klein", "Nurse Lewis", "Nurse Moore", "Nurse Nelson", "Nurse Owens",
        "Nurse Peters", "Nurse Quinn", "Nurse Roberts", "Nurse Smith", "Nurse Thomas",
        "Nurse Rose", "Nurse Ahmad", "Nurse White", "Nurse Xiong", "Nurse Nafiz"
    };
    
    int nurseIndex = 0;
    for (size_t i = 0; i < hospitals.size(); i++) {
        Hospital* hospital = hospitals[i];
        
        // Add 5 nurses to each hospital
        for (int j = 0; j < 5; j++) {
            string nurseID = "N" + to_string(nurseIndex + 1);
            Nurse* nurse = new Nurse(nurseID, NURSE_NAMES[nurseIndex], hospital->hospitalID);
            
            // Add nurse to system and hospital
            nurses[nurseID] = nurse;
            hospital->addNurse(nurse);
            
            nurseIndex++;
        }
    }
}

bool HospitalSystem::admitPatient(Patient* patient, int hospitalIndex) {
    // Validate hospital index
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    // Check if hospital has capacity
    if (hospitals[hospitalIndex]->admitPatient(patient)) {
        patients[patient->patientID] = patient;
        return true;
    }
    return false;
}

bool HospitalSystem::relocatePatient(string patientID, int newHospitalIndex) {
    // Validate hospital index
    if (newHospitalIndex < 0 || static_cast<size_t>(newHospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }
    
    Hospital* currentHospital = findPatientHospital(patientID);
    if (!currentHospital) {
        return false;
    }
    
    return currentHospital->relocatePatient(patient, hospitals[newHospitalIndex]);
}

bool HospitalSystem::dischargePatient(string patientID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }

    if (!patient->dischargeRequested) {
        // The doctor never requested discharge, so do not allow it.
        return false;
    }

    // Otherwise, proceed with discharge
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {
        return false;
    }

    // Actually remove the patient from the hospital
    hospital->dischargePatient(patient);
    patients.erase(patientID);
    delete patient;
    return true;
}

Hospital* HospitalSystem::getHospital(int index) {
    if (index >= 0 && static_cast<size_t>(index) < hospitals.size()) {
        return hospitals[index];
    }
    return nullptr;
}

vector<Hospital*> HospitalSystem::getAllHospitals() {
    return hospitals;
}

map<string, Patient*>& HospitalSystem::getAllPatients() {
    return patients;
}

Patient* HospitalSystem::findPatient(string patientID) {
    auto it = patients.find(patientID);
    return (it != patients.end()) ? it->second : nullptr;
}

Hospital* HospitalSystem::findPatientHospital(string patientID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return nullptr;
    }
    
    for (auto hospital : hospitals) {
        auto it = find(hospital->patients.begin(), hospital->patients.end(), patient);
        if (it != hospital->patients.end()) {
            return hospital;
        }
    }
    return nullptr;
}

Doctor* HospitalSystem::findDoctor(string doctorID) {
    auto it = doctors.find(doctorID);
    return (it != doctors.end()) ? it->second : nullptr;
}

Nurse* HospitalSystem::findNurse(string nurseID) {
    auto it = nurses.find(nurseID);
    return (it != nurses.end()) ? it->second : nullptr;
}

bool HospitalSystem::addDoctor(Doctor* doctor) {
    // Check if we've reached the maximum number of doctors
    if (doctors.size() >= 50) {
        return false;
    }
    
    // Make sure doctor ID is unique
    if (doctors.find(doctor->doctorID) != doctors.end()) {
        return false;
    }
    
    // Find the hospital
    Hospital* hospital = nullptr;
    for (auto h : hospitals) {
        if (h->hospitalID == doctor->hospitalID) {
            hospital = h;
            break;
        }
    }
    
    if (!hospital) {
        return false;
    }
    
    // Add doctor to system and hospital
    doctors[doctor->doctorID] = doctor;
    hospital->addDoctor(doctor);
    
    return true;
}

bool HospitalSystem::addNurse(Nurse* nurse) {
    // Check if we've reached the maximum number of nurses
    if (nurses.size() >= 60) {
        return false;
    }
    
    // Make sure nurse ID is unique
    if (nurses.find(nurse->nurseID) != nurses.end()) {
        return false;
    }
    
    // Find the hospital
    Hospital* hospital = nullptr;
    for (auto h : hospitals) {
        if (h->hospitalID == nurse->hospitalID) {
            hospital = h;
            break;
        }
    }
    
    if (!hospital) {
        return false;
    }
    
    // Add nurse to system and hospital
    nurses[nurse->nurseID] = nurse;
    hospital->addNurse(nurse);
    
    return true;
}

string HospitalSystem::orderPrescription(int hospitalIndex, string patientID, string pharmacyID, string medication, double price) {
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return "";
    }
    
    Hospital* hospital = hospitals[hospitalIndex];
    return hospital->orderPrescription(pharmacyID, patientID, medication, price);
}

bool HospitalSystem::payPharmacyBill(int hospitalIndex, string pharmacyID, string billID) {
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    Hospital* hospital = hospitals[hospitalIndex];
    return hospital->payPharmacyBill(pharmacyID, billID);
}

void HospitalSystem::updateAllPatientDays() {
    for (auto hospital : hospitals) {
        hospital->updatePatientDays();
    }
}

double HospitalSystem::calculatePatientBill(string patientID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return 0.0;
    }
    
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {
        return 0.0;
    }
    
    return hospital->calculatePatientBill(patientID);
}

bool HospitalSystem::collectPatientPayment(string patientID, double amount) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }
    
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {
        return false;
    }
    
    return hospital->collectPaymentFromPatient(patientID, amount);
}

bool HospitalSystem::assignDoctorToPatient(string doctorID, string patientID, bool isPrimary) {
    Doctor* doctor = findDoctor(doctorID);
    if (!doctor) {
        return false;
    }
    
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }
    
    // Find which hospital the doctor is in
    Hospital* doctorHospital = nullptr;
    for (auto hospital : hospitals) {
        for (auto doc : hospital->doctors) {
            if (doc->doctorID == doctorID) {
                doctorHospital = hospital;
                break;
            }
        }
        if (doctorHospital) break;
    }
    
    if (!doctorHospital) {
        return false; // Doctor not found in any hospital
    }
    
    // Find which hospital the patient is in
    Hospital* patientHospital = findPatientHospital(patientID);
    if (!patientHospital) {
        return false; // Patient not found in any hospital
    }
    
    // Doctors can only be assigned to patients in the same hospital
    if (doctorHospital->hospitalID != patientHospital->hospitalID) {
        return false;
    }
    
    // Assign doctor to patient
    doctor->addPatient(patientID);
    
    // Set as primary if required
    if (isPrimary) {
        patient->primaryDoctorID = doctorID;
    } else {
        patient->addAttendingDoctor(doctorID);
    }
    
    return true;
}

bool HospitalSystem::assignNurseToPatient(string nurseID, string patientID) {
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return false;
    }
    
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }

    if (!patient->addAttendingNurse(nurseID) || !nurse->assignPatient(patientID)) {
        return false;
    }
    
    // Check if nurse can take another patient (max 2)
    return true;
}

bool HospitalSystem::setPatientPrimaryDoctor(string patientID, string doctorID) {
    return assignDoctorToPatient(doctorID, patientID, true);
}

bool HospitalSystem::requestPatientDischarge(string doctorID, string patientID) {
    // Find which hospital the doctor is in
    for (auto hospital : hospitals) {
        for (auto doctor : hospital->doctors) {
            if (doctor->doctorID == doctorID) {
                return hospital->requestPatientDischarge(doctorID, patientID);
            }
        }
    }
    
    return false; // Doctor not found
}

string HospitalSystem::getPatientBillingReport() {
    stringstream report;
    
    for (auto hospital : hospitals) {
        report << hospital->getPatientBillingReport() << "\n\n";
    }
    
    return report.str();
}

string HospitalSystem::getHospitalStatus() {
    stringstream status;
    
    for (auto hospital : hospitals) {
        status << "Hospital: " << hospital->hospitalName << "\n";
        status << "Patients Admitted: " << hospital->patients.size() << "/20\n";
        status << "Doctors: " << hospital->doctors.size() << "\n";
        status << "Nurses: " << hospital->nurses.size() << "\n\n";
        
        // Add detailed doctor information
        if (!hospital->doctors.empty()) {
            status << "--- Doctor Details ---\n";
            for (auto doctor : hospital->doctors) {
                status << "ID: " << doctor->doctorID 
                       << ", Name: " << doctor->doctorName 
                       << ", Patients: " << doctor->patientIDs.size() << "\n";
            }
            status << "\n";
        }
        
        // Add detailed nurse information
        if (!hospital->nurses.empty()) {
            status << "--- Nurse Details ---\n";
            for (auto nurse : hospital->nurses) {
                status << "ID: " << nurse->nurseID 
                       << ", Name: " << nurse->nurseName 
                       << ", Patients: " << nurse->patientIDs.size() << "/2\n";
            }
            status << "\n";
        }
        
        // Display patient information
        if (!hospital->patients.empty()) {
            status << "--- Patient Details ---\n";
            for (auto patient : hospital->patients) {
                status << "ID: " << patient->patientID 
                       << ", Name: " << patient->patientName 
                       << ", Phone: " << patient->phoneNumber << "\n";
                status << "   Disease: " << patient->disease 
                       << ", Treatment: " << patient->treatment 
                       << ", Days Admitted: " << patient->daysAdmitted << "\n";
                status << "   Primary Doctor: " << patient->primaryDoctorID << "\n";
                if (!patient->attendingDoctorIDs.empty()) {
                    status << "   Attending Doctors: ";
                    for (auto& docID : patient->attendingDoctorIDs) {
                        status << docID << " ";
                    }
                    status << "\n";
                }
                status << "\n";
            }
        }
    }
    
    return status.str();
}

double HospitalSystem::getPatientRemainingBalance(string patientID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return 0.0;
    }
    
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {
        return 0.0;
    }
    
    return hospital->getPatientRemainingBalance(patientID);
}
