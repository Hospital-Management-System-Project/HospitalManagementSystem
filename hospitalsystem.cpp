#include "hospitalsystem.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "pharmacysystem.h"
#include <QDebug>

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
        "Dr. Mitchell", "Dr. Carter", "Dr. Roberts", "Dr. Yacoub", "Dr. Griffin"
    };

    int doctorIndex = 0;
    int numHospitals = hospitals.size();

    // Step 1: Add 3 doctors per hospital (guaranteed minimum)
    for (int i = 0; i < numHospitals; ++i) {
        for (int j = 0; j < 3; ++j) {
            string doctorID = "D" + to_string(doctorIndex + 1);
            Doctor* doctor = new Doctor(doctorID, doctorNames[doctorIndex], hospitals[i]->getHospitalID());
            doctors[doctorID] = doctor;
            hospitals[i]->addDoctor(doctor);
            ++doctorIndex;
        }
    }

    // Step 2: Randomly assign the rest to any hospital
    while (doctorIndex < 49) { //1 less than 50 for testing
        int randomHospitalIndex = rand() % numHospitals;
        string doctorID = "D" + to_string(doctorIndex + 1);
        Doctor* doctor = new Doctor(doctorID, doctorNames[doctorIndex], hospitals[randomHospitalIndex]->getHospitalID());
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
            Nurse* nurse = new Nurse(nurseID, nurseNames[nurseIndex], hospitals[i]->getHospitalID());
            nurses[nurseID] = nurse;
            hospitals[i]->addNurse(nurse);
            ++nurseIndex;
        }
    }

    // Step 2: Randomly assign the rest to any hospital
    while (nurseIndex < 59) { //1 less than 60 for testing
        int randomHospitalIndex = rand() % numHospitals;
        string nurseID = "N" + to_string(nurseIndex + 1);
        Nurse* nurse = new Nurse(nurseID, nurseNames[nurseIndex], hospitals[randomHospitalIndex]->getHospitalID());
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
        patients[patient->getPatientID()] = patient;
        string assignedNurseID = patient->getAttendingNursesIDs().at(0);
        findNurse(assignedNurseID)->assignPatient(patient->getPatientID());
        findDoctor(patient->getPrimaryDoctorID())->addPatient(patient->getPatientID());
        
        return true;
    }

    return false;
}

// Fix the relocatePatient method to properly handle all aspects of relocation
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
    
    Hospital* newHospital = hospitals[newHospitalIndex];
    
    // Verify patient is not already in the target hospital
    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        return false; // Already in this hospital
    }
    
    // 1. Unassign all doctors from the patient in the old hospital
    vector<string> doctorIDs = patient->getAttendingDoctorIDs();
    string primaryDoctorID = patient->getPrimaryDoctorID();
    
    // Handle primary doctor
    if (!primaryDoctorID.empty()) {
        Doctor* primaryDoctor = findDoctor(primaryDoctorID);
        if (primaryDoctor) {
            primaryDoctor->removePatient(patientID);
        }
    }
    
    // Handle attending doctors
    for (const string& docID : doctorIDs) {
        Doctor* doctor = findDoctor(docID);
        if (doctor) {
            doctor->removePatient(patientID);
        }
    }
    
    // 2. Unassign all nurses from the patient in the old hospital
    vector<string> nurseIDs = patient->getAttendingNursesIDs();
    for (const string& nurseID : nurseIDs) {
        Nurse* nurse = findNurse(nurseID);
        if (nurse) {
            nurse->removePatient(patientID);
        }
    }
    
    // 3. Clear patient's staff assignments
    patient->getAttendingDoctorIDs().clear();
    patient->getAttendingNursesIDs().clear();
    patient->setPrimaryDoctorID("");
    
    // 4. Remove patient from current hospital
    if (!currentHospital->removePatient(patient)) {
        return false;
    }
    
    // 5. Add patient to new hospital
    if (!newHospital->admitPatient(patient)) {
        // If new hospital couldn't admit, re-add to original hospital
        currentHospital->admitPatient(patient);
        return false;
    }
    
    return true;
}

bool HospitalSystem::dischargePatient(string patientID) {
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }

    if (!patient->isDischargeRequested()) {
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

bool HospitalSystem::addDoctor(Doctor* doctor, int hospitalIndex) {
    
    // Validate hospital index
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    // Check if there are more than 50 doctors
    if (doctors.size() >= 50) {
        return false;
    }

    Hospital* hospitalToAssign = getHospital(hospitalIndex);
    hospitalToAssign->addDoctor(doctor);
    doctors.insert({doctor->getDoctorID(), doctor});
    return true;
}

bool HospitalSystem::relocateDoctor(string doctorID, int newHospitalIndex) {
    // Validate hospital index
    if (newHospitalIndex < 0 || static_cast<size_t>(newHospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    Doctor* doctor = findDoctor(doctorID);
    if (!doctor) {
        return false;
    }
    
    Hospital* currentHospital = findDoctorHospital(doctorID);
    if (!currentHospital) {
        return false;
    }
    
    return currentHospital->relocateDoctor(doctor, hospitals[newHospitalIndex]);
}

bool HospitalSystem::removeDoctor(string doctorID) {
    Doctor* doctor = findDoctor(doctorID);
    if (!doctor) {
        // Doctor doesn't exist or has already been removed
        return false;
    }    

    Hospital* currentHospital = findDoctorHospital(doctorID);
    if (!currentHospital) {
        return false;
    }
    
    // Check if the hospital has enough doctors before removing
    if (currentHospital->getDoctors().size() <= 3) {
        // Instead of just returning false, throw an exception with a clear message
        throw std::runtime_error("Cannot remove doctor: Hospital must maintain at least 3 doctors");
    }
    
    // Check if the doctor has patients
    if (!doctor->getPatientIDs().empty()) {
        throw std::runtime_error("Cannot remove doctor with assigned patients");
    }
    
    try {
        // Remove the doctor from the hospital
        currentHospital->removeDoctor(doctor);
        // Also remove the doctor from the system's map
        doctors.erase(doctorID);
        delete doctor;
        return true;
    } catch (const std::runtime_error& e) {
        // Rethrow any exceptions from the hospital's removeDoctor
        throw;
    } catch (...) {
        // Catch any other unexpected exceptions
        return false;
    }
}

bool HospitalSystem::addNurse(Nurse* nurse, int hospitalIndex) {
    // Validate hospital index
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    // Check if there are more than 60 nurses
    if (nurses.size() >= 60) {
        return false;
    }

    Hospital* hospitalToAssign = getHospital(hospitalIndex);
    hospitalToAssign->addNurse(nurse);
    nurses.insert({nurse->getNurseID(), nurse});
    return true;    
}

bool HospitalSystem::relocateNurse(const std::string& nurseID, int newHospitalIndex) {
    // Validate hospital index
    if (newHospitalIndex < 0 || static_cast<size_t>(newHospitalIndex) >= hospitals.size()) {
        return false;
    }
    
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return false;
    }
    
    Hospital* currentHospital = findNurseHospital(nurseID);
    if (!currentHospital) {
        return false;
    }
    
    // Check if nurse has patients
    if (!nurse->getPatientIDs().empty()) {
        return false; // Cannot relocate nurse with patients
    }
    
    // Remove from current hospital's nurse list
    auto& currentNurses = currentHospital->getNurses();
    currentNurses.erase(remove(currentNurses.begin(), currentNurses.end(), nurse), currentNurses.end());
    
    // Update nurse's hospital ID
    nurse->setHospitalID(hospitals[newHospitalIndex]->getHospitalID());
    
    // Add to new hospital
    hospitals[newHospitalIndex]->addNurse(nurse);
    
    return true;
}

bool HospitalSystem::removeNurse(const std::string& nurseID) {
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return false;
    }
    
    Hospital* currentHospital = findNurseHospital(nurseID);
    if (!currentHospital) {
        return false;
    }
    
    // Check if removing this nurse would leave fewer than 3 nurses in the hospital
    if (currentHospital->getNurses().size() <= 3) {
        return false; // Must maintain at least 3 nurses
    }
    
    // Check if nurse has patients
    if (!nurse->getPatientIDs().empty()) {
        return false; // Cannot remove nurse with assigned patients
    }
    
    // Remove from current hospital's nurse list
    auto& currentNurses = const_cast<vector<Nurse*>&>(currentHospital->getNurses());
    currentNurses.erase(remove(currentNurses.begin(), currentNurses.end(), nurse), currentNurses.end());
    
    // Remove from nurses map and delete
    nurses.erase(nurseID);
    delete nurse;
    
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
        auto it = find(hospital->getPatients().begin(), hospital->getPatients().end(), patient);
        if (it != hospital->getPatients().end()) {
            return hospital;
        }
    }
    return nullptr;
}

Hospital* HospitalSystem::findDoctorHospital(string doctorID) {
    Doctor* doctor = findDoctor(doctorID);
    if (!doctor) {
        return nullptr;
    }

    for (auto hospital: hospitals) {
        for (Doctor* doc: hospital->getDoctors()) {
            if (doctor->getDoctorID() == doc->getDoctorID()) {
                return hospital;
            }
        } 
    }
    return nullptr;
}

Hospital* HospitalSystem::findNurseHospital(string nurseID) {
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return nullptr;
    }

    for (auto hospital : hospitals) {
        for (Nurse* nur : hospital->getNurses()) {
            if (nurse->getNurseID() == nur->getNurseID()) {
                return hospital;
            }
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
        for (auto doc : hospital->getDoctors()) {
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
    if (doctorHospital->getHospitalID() != patientHospital->getHospitalID()) {
        return false;
    }
    
    // Assign doctor to patient
    doctor->addPatient(patientID);
    
    // Set as primary if required
    if (isPrimary) {
        patient->setPrimaryDoctorID(doctorID);
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

    if (std::find(nurse->getPatientIDs().begin(), nurse->getPatientIDs().end(), patientID) != nurse->getPatientIDs().end()) {
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
        for (auto doctor : hospital->getDoctors()) {
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
        status << "Hospital: " << hospital->getHospitalName() << "\n";
        status << "Patients Admitted: " << hospital->getPatients().size() << "/20\n";
        status << "Doctors: " << hospital->getDoctors().size() << "\n";
        status << "Nurses: " << hospital->getNurses().size() << "\n\n";
        
        // Add detailed doctor information
        if (!hospital->getDoctors().empty()) {
            status << "--- Doctor Details ---\n";
            for (auto doctor : hospital->getDoctors()) {
                status << "ID: " << doctor->getDoctorID()
                       << ", Name: " << doctor->getDoctorName()
                       << ", Patients: " << doctor->getPatientIDs().size() << "\n";
            }
            status << "\n";
        }
        
        // Add detailed nurse information
        if (!hospital->getNurses().empty()) {
            status << "--- Nurse Details ---\n";
            for (auto nurse : hospital->getNurses()) {
                status << "ID: " << nurse->getNurseID()
                       << ", Name: " << nurse->getNurseName()
                       << ", Patients: " << nurse->getPatientIDs().size() << "/2\n";
            }
            status << "\n";
        }
        
        // Display patient information with improved formatting
        if (!hospital->getPatients().empty()) {
            status << "=================== PATIENT DETAILS ===================\n\n";
            
            for (auto patient : hospital->getPatients()) {
                status << "PATIENT INFORMATION\n";
                status << "Name:   \t\t" << patient->getPatientName() << "\n";
                status << "ID:   \t\t" << patient->getPatientID() << "\n";
                status << "Phone:   \t\t" << patient->getPhoneNumber() << "\n";
                status << "Days Admitted:\t " << patient->getDaysAdmitted() << "\n\n";
                
                status << "MEDICAL INFORMATION\n";
                status << "Diagnosis:      " << patient->getDisease() << "\n";
                status << "Treatment:      " << patient->getTreatment() << "\n\n";
                
                status << "MEDICAL STAFF\n";
                status << "Primary Doctor: " << patient->getPrimaryDoctorID() << "\n";
                
                if (!patient->getAttendingDoctorIDs().empty()) {
                    status << "Attending Doctors:\n";
                    for (auto& docID : patient->getAttendingDoctorIDs()) {
                        status << "  • " << docID << "\n";
                    }
                }
                
                if (!patient->getAttendingNursesIDs().empty()) {
                    status << "Attending Nurses:\n";
                    for (auto& nurseID : patient->getAttendingNursesIDs()) {
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
