#include "hospitalsystem.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
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

    srand(time(nullptr)); // Seed random generator

    // Doctor names (50 total)
    vector<string> doctorNames = {
        "Dr. Smith", "Dr. Johnson", "Dr. Williams", "Dr. Brown", "Dr. Jones",
        "Dr. Miller", "Dr. Davis", "Dr. Garcia", "Dr. Rodriguez", "Dr. Wilson",
        "Dr. Martinez", "Dr. Anderson", "Dr. Taylor", "Dr. Thomas", "Dr. Hernandez",
        "Dr. Moore", "Dr. Jackson", "Dr. Martin", "Dr. Lee", "Dr. Perez",
        "Dr. Thompson", "Dr. White", "Dr. Harris", "Dr. Sanchez", "Dr. Clark",
        "Dr. Ramirez", "Dr. Lewis", "Dr. Robinson", "Dr. Walker", "Dr. Young",
        "Dr. Allen", "Dr. King", "Dr. Wright", "Dr. Scott", "Dr. Torres",
        "Dr. Nguyen", "Dr. Hill", "Dr. Flores", "Dr. Green", "Dr. Adams",
        "Dr. Nelson", "Dr. Baker", "Dr. Hall", "Dr. Rivera", "Dr. Campbell",
        "Dr. Mitchell", "Dr. Carter", "Dr. Roberts", "Dr. Gomez", "Dr. Morgan"
    };

    int doctorIndex = 0;
    int numHospitals = hospitals.size();

    // Step 1: Add 3 doctors per hospital (guaranteed minimum)
    for (int i = 0; i < numHospitals; ++i) {
        for (int j = 0; j < 3; ++j) {
            string doctorID = "D" + to_string(doctorIndex + 1);
            Doctor* doctor = new Doctor(doctorID, doctorNames[doctorIndex], hospitals[i]->hospitalID);
            doctors[doctorID] = doctor;
            hospitals[i]->addDoctor(doctor);
            ++doctorIndex;
        }
    }

    // Step 2: Randomly assign the rest to any hospital
    while (doctorIndex < 50) {
        int randomHospitalIndex = rand() % numHospitals;
        string doctorID = "D" + to_string(doctorIndex + 1);
        Doctor* doctor = new Doctor(doctorID, doctorNames[doctorIndex], hospitals[randomHospitalIndex]->hospitalID);
        doctors[doctorID] = doctor;
        hospitals[randomHospitalIndex]->addDoctor(doctor);
        ++doctorIndex;
    }
}

void HospitalSystem::initializeNurses() {
    // Clear existing nurses
    for (auto& pair : nurses) {
        delete pair.second;
    }
    nurses.clear();

    srand(time(nullptr)); // Seed random generator

    // Nurse names (60 total)
    vector<string> nurseNames = {
        "Nurse Adams", "Nurse Baker", "Nurse Clark", "Nurse Davis", "Nurse Evans",
        "Nurse Foster", "Nurse Green", "Nurse Harris", "Nurse Ivy", "Nurse Jones",
        "Nurse Klein", "Nurse Lewis", "Nurse Moore", "Nurse Nelson", "Nurse Owens",
        "Nurse Peters", "Nurse Quinn", "Nurse Roberts", "Nurse Smith", "Nurse Thomas",
        "Nurse Underwood", "Nurse Vega", "Nurse Wolfe", "Nurse Xu", "Nurse Young",
        "Nurse Zane", "Nurse Allen", "Nurse Brooks", "Nurse Carter", "Nurse Diaz",
        "Nurse Ellis", "Nurse Flores", "Nurse Graham", "Nurse Hunt", "Nurse Irwin",
        "Nurse Jenkins", "Nurse Knight", "Nurse Long", "Nurse Miller", "Nurse Neal",
        "Nurse Ortiz", "Nurse Patel", "Nurse Quinnie", "Nurse Ramirez", "Nurse Stone",
        "Nurse Tran", "Nurse Usman", "Nurse Valentine", "Nurse Washington", "Nurse Xiong",
        "Nurse Yates", "Nurse Zahra", "Nurse Bell", "Nurse Chang", "Nurse Dalton",
        "Nurse Edmonds", "Nurse Fong", "Nurse Garcia", "Nurse Harper", "Nurse Nafiz"
    };

    int nurseIndex = 0;
    int numHospitals = hospitals.size();

    // Step 1: Add 5 nurses per hospital (guaranteed minimum)
    for (int i = 0; i < numHospitals; ++i) {
        for (int j = 0; j < 5; ++j) {
            string nurseID = "N" + to_string(nurseIndex + 1);
            Nurse* nurse = new Nurse(nurseID, nurseNames[nurseIndex], hospitals[i]->hospitalID);
            nurses[nurseID] = nurse;
            hospitals[i]->addNurse(nurse);
            ++nurseIndex;
        }
    }

    // Step 2: Randomly assign the rest to any hospital
    while (nurseIndex < 60) {
        int randomHospitalIndex = rand() % numHospitals;
        string nurseID = "N" + to_string(nurseIndex + 1);
        Nurse* nurse = new Nurse(nurseID, nurseNames[nurseIndex], hospitals[randomHospitalIndex]->hospitalID);
        nurses[nurseID] = nurse;
        hospitals[randomHospitalIndex]->addNurse(nurse);
        ++nurseIndex;
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

        // add patient to 'patientIDs' vector for the nurse
        string assignedNurseID = patient->attendingNursesIDs.at(0);
        findNurse(assignedNurseID)->patientIDs.push_back(patient->patientID);

        // add patient to 'patientIDs' vector for the doctor
        findDoctor(patient->primaryDoctorID)->getPatientIDs().push_back(patient->patientID);
        
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

void HospitalSystem::updateAllPatientDays() {
    for (auto hospital : hospitals) {
        hospital->updatePatientDays();
    }
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
            if (doc->getDoctorID() == doctorID) {
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

    if (std::find(nurse->patientIDs.begin(), nurse->patientIDs.end(), patientID) != nurse->patientIDs.end()) {
        return false;
    }

    if (!nurse->assignPatient(patientID)) {
        return false;
    }
    
    patient->addAttendingNurse(nurseID);
    return true;
}

bool HospitalSystem::setPatientPrimaryDoctor(string patientID, string doctorID) {
    return assignDoctorToPatient(doctorID, patientID, true);
}

bool HospitalSystem::requestPatientDischarge(string doctorID, string patientID) {
    // Find which hospital the doctor is in
    for (auto hospital : hospitals) {
        for (auto doctor : hospital->doctors) {
            if (doctor->getDoctorID() == doctorID) {
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
                status << "ID: " << doctor->getDoctorID()
                       << ", Name: " << doctor->getDoctorName()
                       << ", Patients: " << doctor->getPatientIDs().size() << "\n";
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
        
        // Display patient information with improved formatting
        if (!hospital->patients.empty()) {
            status << "=================== PATIENT DETAILS ===================\n\n";
            
            for (auto patient : hospital->patients) {
                status << "PATIENT INFORMATION\n";
                status << "Name:   \t\t" << patient->patientName << "\n";
                status << "ID:   \t\t" << patient->patientID << "\n";
                status << "Phone:   \t\t" << patient->phoneNumber << "\n";
                status << "Days Admitted:\t " << patient->daysAdmitted << "\n\n";
                
                status << "MEDICAL INFORMATION\n";
                status << "Diagnosis:      " << patient->disease << "\n";
                status << "Treatment:      " << patient->treatment << "\n\n";
                
                status << "MEDICAL STAFF\n";
                status << "Primary Doctor: " << patient->primaryDoctorID << "\n";
                
                if (!patient->attendingDoctorIDs.empty()) {
                    status << "Attending Doctors:\n";
                    for (auto& docID : patient->attendingDoctorIDs) {
                        status << "  • " << docID << "\n";
                    }
                }
                
                if (!patient->attendingNursesIDs.empty()) {
                    status << "Attending Nurses:\n";
                    for (auto& nurseID : patient->attendingNursesIDs) {
                        status << "  • " << nurseID << "\n";
                    }
                }
                
                status << "======================================================\n\n";
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
